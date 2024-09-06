// SPDX-License-Identifier: GPL-2.0
/*
 * xhci-dbgtty.c - tty glue for xHCI debug capability
 *
 * Copyright (C) 2017 Intel Corporation
 *
 * Author: Lu Baolu <baolu.lu@linux.intel.com>
 */

#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>

#include "xhci-meson.h"
#include "xhci-dbgcap-meson.h"

static int dbc_tty_init(void);
static void dbc_tty_exit(void);

static struct tty_driver *dbc_tty_driver;

static inline struct aml_dbc_port *dbc_to_port(struct aml_xhci_dbc *dbc)
{
	return dbc->priv;
}

static unsigned int
dbc_send_packet(struct aml_dbc_port *port, char *packet, unsigned int size)
{
	unsigned int		len;

	len = kfifo_len(&port->write_fifo);
	if (len < size)
		size = len;
	if (size != 0)
		size = kfifo_out(&port->write_fifo, packet, size);
	return size;
}

static int dbc_start_tx(struct aml_dbc_port *port)
	__releases(&port->port_lock)
	__acquires(&port->port_lock)
{
	int			len;
	struct aml_dbc_request	*req;
	int			status = 0;
	bool			do_tty_wake = false;
	struct list_head	*pool = &port->write_pool;

	while (!list_empty(pool)) {
		req = list_entry(pool->next, struct aml_dbc_request, list_pool);
		len = dbc_send_packet(port, req->buf, DBC_MAX_PACKET);
		if (len == 0)
			break;
		do_tty_wake = true;

		req->length = len;
		list_del(&req->list_pool);

		spin_unlock(&port->port_lock);
		status = aml_dbc_ep_queue(req);
		spin_lock(&port->port_lock);

		if (status) {
			list_add(&req->list_pool, pool);
			break;
		}
	}

	if (do_tty_wake && port->port.tty)
		tty_wakeup(port->port.tty);

	return status;
}

static void dbc_start_rx(struct aml_dbc_port *port)
	__releases(&port->port_lock)
	__acquires(&port->port_lock)
{
	struct aml_dbc_request	*req;
	int			status;
	struct list_head	*pool = &port->read_pool;

	while (!list_empty(pool)) {
		if (!port->port.tty)
			break;

		req = list_entry(pool->next, struct aml_dbc_request, list_pool);
		list_del(&req->list_pool);
		req->length = DBC_MAX_PACKET;

		spin_unlock(&port->port_lock);
		status = aml_dbc_ep_queue(req);
		spin_lock(&port->port_lock);

		if (status) {
			list_add(&req->list_pool, pool);
			break;
		}
	}
}

static void
dbc_read_complete(struct aml_xhci_dbc *dbc, struct aml_dbc_request *req)
{
	unsigned long		flags;
	struct aml_dbc_port		*port = dbc_to_port(dbc);

	spin_lock_irqsave(&port->port_lock, flags);
	list_add_tail(&req->list_pool, &port->read_queue);
	tasklet_schedule(&port->push);
	spin_unlock_irqrestore(&port->port_lock, flags);
}

static void dbc_write_complete(struct aml_xhci_dbc *dbc, struct aml_dbc_request *req)
{
	unsigned long		flags;
	struct aml_dbc_port		*port = dbc_to_port(dbc);

	spin_lock_irqsave(&port->port_lock, flags);
	list_add(&req->list_pool, &port->write_pool);
	switch (req->status) {
	case 0:
		dbc_start_tx(port);
		break;
	case -ESHUTDOWN:
		break;
	default:
		dev_warn(dbc->dev, "unexpected write complete status %d\n",
			  req->status);
		break;
	}
	spin_unlock_irqrestore(&port->port_lock, flags);
}

static void xhci_dbc_free_req(struct aml_dbc_request *req)
{
	kfree(req->buf);
	aml_dbc_free_request(req);
}

static int
xhci_dbc_alloc_requests(struct aml_xhci_dbc *dbc, unsigned int direction,
			struct list_head *head,
			void (*fn)(struct aml_xhci_dbc *, struct aml_dbc_request *))
{
	int			i;
	struct aml_dbc_request	*req;

	for (i = 0; i < DBC_QUEUE_SIZE; i++) {
		req = aml_dbc_alloc_request(dbc, direction, GFP_KERNEL);
		if (!req)
			break;

		req->length = DBC_MAX_PACKET;
		req->buf = kmalloc(req->length, GFP_KERNEL);
		if (!req->buf) {
			aml_dbc_free_request(req);
			break;
		}

		req->complete = fn;
		list_add_tail(&req->list_pool, head);
	}

	return list_empty(head) ? -ENOMEM : 0;
}

static void
xhci_dbc_free_requests(struct list_head *head)
{
	struct aml_dbc_request	*req;

	while (!list_empty(head)) {
		req = list_entry(head->next, struct aml_dbc_request, list_pool);
		list_del(&req->list_pool);
		xhci_dbc_free_req(req);
	}
}

static int dbc_tty_install(struct tty_driver *driver, struct tty_struct *tty)
{
	struct aml_dbc_port		*port = driver->driver_state;

	tty->driver_data = port;

	return tty_port_install(&port->port, driver, tty);
}

static int dbc_tty_open(struct tty_struct *tty, struct file *file)
{
	struct aml_dbc_port		*port = tty->driver_data;

	return tty_port_open(&port->port, tty, file);
}

static void dbc_tty_close(struct tty_struct *tty, struct file *file)
{
	struct aml_dbc_port		*port = tty->driver_data;

	tty_port_close(&port->port, tty, file);
}

static int dbc_tty_write(struct tty_struct *tty,
			 const unsigned char *buf,
			 int count)
{
	struct aml_dbc_port		*port = tty->driver_data;
	unsigned long		flags;

	spin_lock_irqsave(&port->port_lock, flags);
	if (count)
		count = kfifo_in(&port->write_fifo, buf, count);
	dbc_start_tx(port);
	spin_unlock_irqrestore(&port->port_lock, flags);

	return count;
}

static int dbc_tty_put_char(struct tty_struct *tty, unsigned char ch)
{
	struct aml_dbc_port		*port = tty->driver_data;
	unsigned long		flags;
	int			status;

	spin_lock_irqsave(&port->port_lock, flags);
	status = kfifo_put(&port->write_fifo, ch);
	spin_unlock_irqrestore(&port->port_lock, flags);

	return status;
}

static void dbc_tty_flush_chars(struct tty_struct *tty)
{
	struct aml_dbc_port		*port = tty->driver_data;
	unsigned long		flags;

	spin_lock_irqsave(&port->port_lock, flags);
	dbc_start_tx(port);
	spin_unlock_irqrestore(&port->port_lock, flags);
}

static unsigned int dbc_tty_write_room(struct tty_struct *tty)
{
	struct aml_dbc_port		*port = tty->driver_data;
	unsigned long		flags;
	unsigned int		room;

	spin_lock_irqsave(&port->port_lock, flags);
	room = kfifo_avail(&port->write_fifo);
	spin_unlock_irqrestore(&port->port_lock, flags);

	return room;
}

static unsigned int dbc_tty_chars_in_buffer(struct tty_struct *tty)
{
	struct aml_dbc_port		*port = tty->driver_data;
	unsigned long		flags;
	unsigned int		chars;

	spin_lock_irqsave(&port->port_lock, flags);
	chars = kfifo_len(&port->write_fifo);
	spin_unlock_irqrestore(&port->port_lock, flags);

	return chars;
}

static void dbc_tty_unthrottle(struct tty_struct *tty)
{
	struct aml_dbc_port		*port = tty->driver_data;
	unsigned long		flags;

	spin_lock_irqsave(&port->port_lock, flags);
	tasklet_schedule(&port->push);
	spin_unlock_irqrestore(&port->port_lock, flags);
}

static const struct tty_operations dbc_tty_ops = {
	.install		= dbc_tty_install,
	.open			= dbc_tty_open,
	.close			= dbc_tty_close,
	.write			= dbc_tty_write,
	.put_char		= dbc_tty_put_char,
	.flush_chars		= dbc_tty_flush_chars,
	.write_room		= dbc_tty_write_room,
	.chars_in_buffer	= dbc_tty_chars_in_buffer,
	.unthrottle		= dbc_tty_unthrottle,
};

static void dbc_rx_push(struct tasklet_struct *t)
{
	struct aml_dbc_request	*req;
	struct tty_struct	*tty;
	unsigned long		flags;
	bool			do_push = false;
	bool			disconnect = false;
	struct aml_dbc_port		*port = from_tasklet(port, t, push);
	struct list_head	*queue = &port->read_queue;

	spin_lock_irqsave(&port->port_lock, flags);
	tty = port->port.tty;
	while (!list_empty(queue)) {
		req = list_first_entry(queue, struct aml_dbc_request, list_pool);

		if (tty && tty_throttled(tty))
			break;

		switch (req->status) {
		case 0:
			break;
		case -ESHUTDOWN:
			disconnect = true;
			break;
		default:
			pr_warn("ttyDBC0: unexpected RX status %d\n",
				req->status);
			break;
		}

		if (req->actual) {
			char		*packet = req->buf;
			unsigned int	n, size = req->actual;
			int		count;

			n = port->n_read;
			if (n) {
				packet += n;
				size -= n;
			}

			count = tty_insert_flip_string(&port->port, packet,
						       size);
			if (count)
				do_push = true;
			if (count != size) {
				port->n_read += count;
				break;
			}
			port->n_read = 0;
		}

		list_move(&req->list_pool, &port->read_pool);
	}

	if (do_push)
		tty_flip_buffer_push(&port->port);

	if (!list_empty(queue) && tty) {
		if (!tty_throttled(tty)) {
			if (do_push)
				tasklet_schedule(&port->push);
			else
				pr_warn("ttyDBC0: RX not scheduled?\n");
		}
	}

	if (!disconnect)
		dbc_start_rx(port);

	spin_unlock_irqrestore(&port->port_lock, flags);
}

static int dbc_port_activate(struct tty_port *_port, struct tty_struct *tty)
{
	unsigned long	flags;
	struct aml_dbc_port	*port = container_of(_port, struct aml_dbc_port, port);

	spin_lock_irqsave(&port->port_lock, flags);
	dbc_start_rx(port);
	spin_unlock_irqrestore(&port->port_lock, flags);

	return 0;
}

static const struct tty_port_operations dbc_port_ops = {
	.activate =	dbc_port_activate,
};

static void
xhci_dbc_tty_init_port(struct aml_xhci_dbc *dbc, struct aml_dbc_port *port)
{
	tty_port_init(&port->port);
	spin_lock_init(&port->port_lock);
	tasklet_setup(&port->push, dbc_rx_push);
	INIT_LIST_HEAD(&port->read_pool);
	INIT_LIST_HEAD(&port->read_queue);
	INIT_LIST_HEAD(&port->write_pool);

	port->port.ops =	&dbc_port_ops;
	port->n_read =		0;
}

static void
xhci_dbc_tty_exit_port(struct aml_dbc_port *port)
{
	tasklet_kill(&port->push);
	tty_port_destroy(&port->port);
}

static int xhci_dbc_tty_register_device(struct aml_xhci_dbc *dbc)
{
	int			ret;
	struct device		*tty_dev;
	struct aml_dbc_port		*port = dbc_to_port(dbc);

	if (port->registered)
		return -EBUSY;

	xhci_dbc_tty_init_port(dbc, port);

	ret = kfifo_alloc(&port->write_fifo, DBC_WRITE_BUF_SIZE, GFP_KERNEL);
	if (ret)
		goto err_exit_port;

	ret = xhci_dbc_alloc_requests(dbc, BULK_IN, &port->read_pool,
				      dbc_read_complete);
	if (ret)
		goto err_free_fifo;

	ret = xhci_dbc_alloc_requests(dbc, BULK_OUT, &port->write_pool,
				      dbc_write_complete);
	if (ret)
		goto err_free_requests;

	tty_dev = tty_port_register_device(&port->port,
					   dbc_tty_driver, 0, NULL);
	if (IS_ERR(tty_dev)) {
		ret = PTR_ERR(tty_dev);
		goto err_free_requests;
	}

	port->registered = true;

	return 0;

err_free_requests:
	xhci_dbc_free_requests(&port->read_pool);
	xhci_dbc_free_requests(&port->write_pool);
err_free_fifo:
	kfifo_free(&port->write_fifo);
err_exit_port:
	xhci_dbc_tty_exit_port(port);

	dev_err(dbc->dev, "can't register tty port, err %d\n", ret);

	return ret;
}

static void xhci_dbc_tty_unregister_device(struct aml_xhci_dbc *dbc)
{
	struct aml_dbc_port		*port = dbc_to_port(dbc);

	if (!port->registered)
		return;
	tty_unregister_device(dbc_tty_driver, 0);
	xhci_dbc_tty_exit_port(port);
	port->registered = false;

	kfifo_free(&port->write_fifo);
	xhci_dbc_free_requests(&port->read_pool);
	xhci_dbc_free_requests(&port->read_queue);
	xhci_dbc_free_requests(&port->write_pool);
}

static const struct aml_dbc_driver dbc_driver = {
	.configure		= xhci_dbc_tty_register_device,
	.disconnect		= xhci_dbc_tty_unregister_device,
};

int aml_xhci_dbc_tty_probe(struct device *dev, void __iomem *base, struct aml_xhci_hcd *xhci)
{
	struct aml_xhci_dbc		*dbc;
	struct aml_dbc_port		*port;
	int			status;

	/* dbc_tty_init will be called by module init() in the future */
	status = dbc_tty_init();
	if (status)
		return status;

	port = kzalloc(sizeof(*port), GFP_KERNEL);
	if (!port) {
		status = -ENOMEM;
		goto out;
	}

	dbc_tty_driver->driver_state = port;

	dbc = aml_xhci_alloc_dbc(dev, base, &dbc_driver);
	if (!dbc) {
		status = -ENOMEM;
		goto out2;
	}

	dbc->priv = port;

	/* get rid of xhci once this is a real driver binding to a device */
	xhci->dbc = dbc;

	return 0;
out2:
	kfree(port);
out:
	/* dbc_tty_exit will be called by module_exit() in the future */
	dbc_tty_exit();
	return status;
}

/*
 * undo what probe did, assume dbc is stopped already.
 * we also assume tty_unregister_device() is called before this
 */
void aml_xhci_dbc_tty_remove(struct aml_xhci_dbc *dbc)
{
	struct aml_dbc_port         *port = dbc_to_port(dbc);

	aml_xhci_dbc_remove(dbc);
	kfree(port);

	/* dbc_tty_exit will be called by  module_exit() in the future */
	dbc_tty_exit();
}

static int dbc_tty_init(void)
{
	int		ret;

	dbc_tty_driver = tty_alloc_driver(1, TTY_DRIVER_REAL_RAW |
					  TTY_DRIVER_DYNAMIC_DEV);
	if (IS_ERR(dbc_tty_driver))
		return PTR_ERR(dbc_tty_driver);

	dbc_tty_driver->driver_name = "dbc_serial";
	dbc_tty_driver->name = "ttyDBC";

	dbc_tty_driver->type = TTY_DRIVER_TYPE_SERIAL;
	dbc_tty_driver->subtype = SERIAL_TYPE_NORMAL;
	dbc_tty_driver->init_termios = tty_std_termios;
	dbc_tty_driver->init_termios.c_cflag =
			B9600 | CS8 | CREAD | HUPCL | CLOCAL;
	dbc_tty_driver->init_termios.c_ispeed = 9600;
	dbc_tty_driver->init_termios.c_ospeed = 9600;

	tty_set_operations(dbc_tty_driver, &dbc_tty_ops);

	ret = tty_register_driver(dbc_tty_driver);
	if (ret) {
		pr_err("Can't register dbc tty driver\n");
		tty_driver_kref_put(dbc_tty_driver);
	}
	return ret;
}

static void dbc_tty_exit(void)
{
	if (dbc_tty_driver) {
		tty_unregister_driver(dbc_tty_driver);
		tty_driver_kref_put(dbc_tty_driver);
		dbc_tty_driver = NULL;
	}
}