/* cfake.c - implementation of a simple module for a character device 
 * can be used for testing, demonstrations, etc.
 */

/* ========================================================================
 * Copyright (C) 2010-2011, Institute for System Programming 
 *                          of the Russian Academy of Sciences (ISPRAS)
 * Authors: 
 *      Eugene A. Shatokhin <spectre@ispras.ru>
 *      Andrey V. Tsyvarev  <tsyvarev@ispras.ru>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 ======================================================================== */

#include <linux/version.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/mutex.h>

#include <linux/wait.h>   // Sleeping
#include <linux/sched.h>  // Jiffies and timing
#include <linux/param.h>  // Holds the HZ
#include <linux/jiffies.h>

#include <asm/uaccess.h>
#include <linux/proc_fs.h>


#include "sleepy.h"

MODULE_AUTHOR("Eugene A. Shatokhin, John Regehr");
MODULE_LICENSE("GPL");

#define SLEEPY_DEVICE_NAME "sleepy"

/* MY CODE */
// Note to grader.  Most of this code is copied from chapter 6/7 of linux
// Device Drivers suggested in the assignment.
//wait_queue_head_t
/*
wait_queue_head_t wq0;
wait_queue_head_t wq1;
wait_queue_head_t wq2;
wait_queue_head_t wq3;
wait_queue_head_t wq4;
wait_queue_head_t wq5;
wait_queue_head_t wq6;
wait_queue_head_t wq7;
wait_queue_head_t wq8;
wait_queue_head_t wq9;
 */

int new0 = 0;
int new1 = 0;
int new2 = 0;
int new3 = 0;
int new4 = 0;
int new5 = 0;
int new6 = 0;
int new7 = 0;
int new8 = 0;
int new9 = 0;

int old0 = 0;
int old1 = 0;
int old2 = 0;
int old3 = 0;
int old4 = 0;
int old5 = 0;
int old6 = 0;
int old7 = 0;
int old8 = 0;
int old9 = 0;


static DECLARE_WAIT_QUEUE_HEAD(wq0);
static DECLARE_WAIT_QUEUE_HEAD(wq1);
static DECLARE_WAIT_QUEUE_HEAD(wq2);
static DECLARE_WAIT_QUEUE_HEAD(wq3);
static DECLARE_WAIT_QUEUE_HEAD(wq4);
static DECLARE_WAIT_QUEUE_HEAD(wq5);
static DECLARE_WAIT_QUEUE_HEAD(wq6);
static DECLARE_WAIT_QUEUE_HEAD(wq7);
static DECLARE_WAIT_QUEUE_HEAD(wq8);
static DECLARE_WAIT_QUEUE_HEAD(wq9);


//static int flag = 0;
// Need separate wait queues
// How to tell what device we are on?
// What is the input?  How to tell what is written
/* MY CODE */

/* parameters */
static int sleepy_ndevices = SLEEPY_NDEVICES;

module_param(sleepy_ndevices, int, S_IRUGO);
/* ================================================================ */

static unsigned int sleepy_major = 0;
static struct sleepy_dev *sleepy_devices = NULL;
static struct class *sleepy_class = NULL;
/* ================================================================ */

int 
sleepy_open(struct inode *inode, struct file *filp)
{
  unsigned int mj = imajor(inode);
  unsigned int mn = iminor(inode);
	
  struct sleepy_dev *dev = NULL;
	
  if (mj != sleepy_major || mn < 0 || mn >= sleepy_ndevices)
    {
      printk(KERN_WARNING "[target] "
	     "No device found with minor=%d and major=%d\n", 
	     mj, mn);
      return -ENODEV; /* No such device */
    }
	
  /* store a pointer to struct sleepy_dev here for other methods */
  dev = &sleepy_devices[mn];
  filp->private_data = dev; 

  if (inode->i_cdev != &dev->cdev)
    {
      printk(KERN_WARNING "[target] open: internal error\n");
      return -ENODEV; /* No such device */
    }
	
  return 0;
}

int 
sleepy_release(struct inode *inode, struct file *filp)
{
  return 0;
}

ssize_t 
sleepy_read(struct file *filp, char __user *buf, size_t count, 
	    loff_t *f_pos)
{
  struct sleepy_dev *dev = (struct sleepy_dev *)filp->private_data;
  ssize_t retval = 0;
	
  if (mutex_lock_killable(&dev->sleepy_mutex))
    return -EINTR;
	
  /* YOUR CODE HERE */
    //printk(KERN_DEBUG "process %i (%s) awakening writers\n", current->pid, current->comm);
    printk("SLEEPY_WRITE DEVICE (%d): remaining = zd \n", dev->id);
    //printk(KERN_DEBUG "Writing Device ID %d\n", dev->id);
    
    
    switch (dev->id) {
        case 0:
            new0 = old0 + 1;
            wake_up_interruptible(&wq0);
            break;
        case 1:
            new1 += old1 + 1;
            wake_up_interruptible(&wq1);
            break;
        case 2:
            new2 += old2 + 1;
            wake_up_interruptible(&wq2);
            break;
        case 3:
            new3 += old3 + 1;
            wake_up_interruptible(&wq3);
            break;
        case 4:
            new4 += old4 + 1;
            wake_up_interruptible(&wq4);
            break;
        case 5:
            new5 += old5 + 1;
            wake_up_interruptible(&wq5);
            break;
        case 6:
            new6 += old6 + 1;
            wake_up_interruptible(&wq6);
            break;
        case 7:
            new7 += old7 + 1;
            wake_up_interruptible(&wq7);
            break;
        case 8:
            new8 += old8 + 1;
            wake_up_interruptible(&wq8);
            break;
        case 9:
            new9 += old9 + 1;
            wake_up_interruptible(&wq9);
            break;
    }
    
    //printk(KERN_DEBUG "process writes to dev id %d\n", dev.id);
    /*
    flag = 1;
    wake_up_interruptible(&wq);
     */
    return count;
  /* END YOUR CODE */
	
  mutex_unlock(&dev->sleepy_mutex);
  return retval;
}
                
ssize_t 
sleepy_write(struct file *filp, const char __user *buf, size_t count, 
	     loff_t *f_pos)
{
  // declared variables
  int id;
  unsigned int timeout;
  unsigned int seconds;
    
  struct sleepy_dev *dev = (struct sleepy_dev *)filp->private_data;
  ssize_t retval = 0;
	
  if (mutex_lock_killable(&dev->sleepy_mutex))
    return -EINTR;
	
  /* YOUR CODE HERE */
    // Modify dev with wait quque
    // __user is the input
    id = dev->id;
    printk(KERN_DEBUG "SLEEPY_READ DEVICE (%d): Process is waking everyone up\n", dev->id);
    //printk(KERN_DEBUG "process %i (%s) sleeping devices\n", current->pid, current->comm);
    //printk(KERN_DEBUG "Reading Device ID %d\n", id);
    
    // Get user input and convert it to a 32 bit integer
    //seconds = *((unsigned int *) buf);
    seconds = 10;
    //printk(KERN_DEBUG "WRITE VAL %d\n", seconds);
    
    // calculate timeout
    timeout = msecs_to_jiffies(1000 * seconds);
    //printk(KERN_DEBUG "timeout %ud\n", timeout);
    
    // Check which device is being written to and print out device id
    switch (dev->id) {
        case 0:
            old0 = new0;
            retval = wait_event_interruptible_timeout(wq0, new0 != old0, timeout);
            break;
        case 1:
            old1 = new1;
            retval = wait_event_interruptible_timeout(wq1, new1 != old1, timeout);
            break;
        case 2:
            old2 = new2;
            retval = wait_event_interruptible_timeout(wq2, new2 != old2, timeout);
            break;
        case 3:
            old3 = new3;
            retval = wait_event_interruptible_timeout(wq3, new3 != old3, timeout);
            break;
        case 4:
            old4 = new4;
            retval = wait_event_interruptible_timeout(wq4, new4 != old4, timeout);
            break;
        case 5:
            old5 = new5;
            retval = wait_event_interruptible_timeout(wq5, new5 != old5, timeout);
            break;
        case 6:
            old6 = new6;
            retval = wait_event_interruptible_timeout(wq6, new6 != old6, timeout);
            break;
        case 7:
            old7 = new7;
            retval = wait_event_interruptible_timeout(wq7, new7 != old7, timeout);
            break;
        case 8:
            old8 = new8;
            retval = wait_event_interruptible_timeout(wq8, new8 != old8, timeout);
            break;
        case 9:
            old9 = new9;
            retval = wait_event_interruptible_timeout(wq9, new9 != old9, timeout);
            break;
    }
    /*
    if (atoi(buff) > 0) {
        // PUT TO SLEEP
        // seconds = jiffies + HZ
        int timeout = atoi(buff) * HZ;  // seconds to jiffies conversion
        retval = wait_event_interruptible_timeout(wq, 0, timeout);
        flag = 0;
        printk(KERN_DEBUG "process %i (%s) awoken\n", current->pid, current->comm);
    } else {
        return EINVAL;
    }
    
    // If we terminate normally
    if (retval == 0) {
        return retval;
    // Return the time in seconds that we still had to wait.
    } else {
        return retval / HZ;
    }
     */
     
  /* END YOUR CODE */
    
	
  mutex_unlock(&dev->sleepy_mutex);
  return retval;
}

loff_t 
sleepy_llseek(struct file *filp, loff_t off, int whence)
{
  return 0;
}

struct file_operations sleepy_fops = {
  .owner =    THIS_MODULE,
  .read =     sleepy_read,
  .write =    sleepy_write,
  .open =     sleepy_open,
  .release =  sleepy_release,
  .llseek =   sleepy_llseek,
};

/* ================================================================ */
/* Setup and register the device with specific index (the index is also
 * the minor number of the device).
 * Device class should be created beforehand.
 */
static int
sleepy_construct_device(struct sleepy_dev *dev, int minor, 
			struct class *class)
{
  int err = 0;
  dev_t devno = MKDEV(sleepy_major, minor);
  struct device *device = NULL;
    
  BUG_ON(dev == NULL || class == NULL);

  /* Memory is to be allocated when the device is opened the first time */
  dev->data = NULL;
  printk(KERN_DEBUG "module id is %d", minor);
  dev->id = minor;    //  Add id to each device
  printk(KERN_DEBUG "dev id is %d", dev->id);
  mutex_init(&dev->sleepy_mutex);
    
  cdev_init(&dev->cdev, &sleepy_fops);
  dev->cdev.owner = THIS_MODULE;
    
  err = cdev_add(&dev->cdev, devno, 1);
  if (err)
    {
      printk(KERN_WARNING "[target] Error %d while trying to add %s%d",
	     err, SLEEPY_DEVICE_NAME, minor);
      return err;
    }

  device = device_create(class, NULL, /* no parent device */ 
			 devno, NULL, /* no additional data */
			 SLEEPY_DEVICE_NAME "%d", minor);

  if (IS_ERR(device)) {
    err = PTR_ERR(device);
    printk(KERN_WARNING "[target] Error %d while trying to create %s%d",
	   err, SLEEPY_DEVICE_NAME, minor);
    cdev_del(&dev->cdev);
    return err;
  }
  return 0;
}

/* Destroy the device and free its buffer */
static void
sleepy_destroy_device(struct sleepy_dev *dev, int minor,
		      struct class *class)
{
  BUG_ON(dev == NULL || class == NULL);
  device_destroy(class, MKDEV(sleepy_major, minor));
  cdev_del(&dev->cdev);
  kfree(dev->data);
  return;
}

/* ================================================================ */
static void
sleepy_cleanup_module(int devices_to_destroy)
{
  int i;
	
  /* Get rid of character devices (if any exist) */
  if (sleepy_devices) {
    for (i = 0; i < devices_to_destroy; ++i) {
      sleepy_destroy_device(&sleepy_devices[i], i, sleepy_class);
    }
    kfree(sleepy_devices);
  }
    
  if (sleepy_class)
    class_destroy(sleepy_class);

  /* [NB] sleepy_cleanup_module is never called if alloc_chrdev_region()
   * has failed. */
  unregister_chrdev_region(MKDEV(sleepy_major, 0), sleepy_ndevices);
  return;
}

static int __init
sleepy_init_module(void)
{
  int err = 0;
  int i = 0;
  int devices_to_destroy = 0;
  dev_t dev = 0;
	
  if (sleepy_ndevices <= 0)
    {
      printk(KERN_WARNING "[target] Invalid value of sleepy_ndevices: %d\n", 
	     sleepy_ndevices);
      err = -EINVAL;
      return err;
    }
	
  /* Get a range of minor numbers (starting with 0) to work with */
  err = alloc_chrdev_region(&dev, 0, sleepy_ndevices, SLEEPY_DEVICE_NAME);
  if (err < 0) {
    printk(KERN_WARNING "[target] alloc_chrdev_region() failed\n");
    return err;
  }
  sleepy_major = MAJOR(dev);

  /* Create device class (before allocation of the array of devices) */
  sleepy_class = class_create(THIS_MODULE, SLEEPY_DEVICE_NAME);
  if (IS_ERR(sleepy_class)) {
    err = PTR_ERR(sleepy_class);
    goto fail;
  }
	
  /* Allocate the array of devices */
  sleepy_devices = (struct sleepy_dev *)kzalloc(
						sleepy_ndevices * sizeof(struct sleepy_dev), 
						GFP_KERNEL);
  if (sleepy_devices == NULL) {
    err = -ENOMEM;
    goto fail;
  }
	
  /* Construct devices */
  for (i = 0; i < sleepy_ndevices; ++i) {
    err = sleepy_construct_device(&sleepy_devices[i], i, sleepy_class);
    if (err) {
      devices_to_destroy = i;
      goto fail;
    }
  }
  
  printk ("sleepy module loaded\n");

  return 0; /* success */

 fail:
  sleepy_cleanup_module(devices_to_destroy);
  printk ("sleepy module failed to load\n");
  return err;
}

static void __exit
sleepy_exit_module(void)
{
  sleepy_cleanup_module(sleepy_ndevices);
  printk ("sleepy module unloaded\n");
  return;
}

module_init(sleepy_init_module);
module_exit(sleepy_exit_module);
/* ================================================================ */
