#include <linux/miscdevice.h>  
#include <linux/delay.h>  
#include <linux/kernel.h>  
#include <linux/module.h>  
#include <linux/init.h>  
#include <linux/mm.h>  
#include <linux/fs.h>  
#include <linux/types.h>  
#include <linux/delay.h>  
#include <linux/moduleparam.h>  
#include <linux/slab.h>  
#include <linux/errno.h>  
#include <linux/ioctl.h>  
#include <linux/cdev.h>  
#include <linux/string.h>  
#include <linux/list.h>  
#include <linux/pci.h>  
#include <linux/gpio.h>  
#define DEVICE_NAME "mymap"

static unsigned char array[] = "Successly mapping kernel space memory to user space memory";

// Pointer for dynamically allocated memory(kernl space)
static unsigned char *buffer;


static int my_open(struct inode *inode, struct file *file)
{
    return 0;
}

// mmap callback for the device
static int my_map(struct file *filp, struct vm_area_struct *vma)
{
	// Physical page frame number
	unsigned long page;
	// Start address of the user-space mapping
	unsigned long start = (unsigned long)vma->vm_start;
	// Size of the mapping
	unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start); 
	
	// check if size exceeds a single page
    	if (size > PAGE_SIZE)
		return -EINVAL;

	// Convert kernel virtual address to physical page frame number
	page = virt_to_phys(buffer) >> PAGE_SHIFT;

	// Map the kernel memory to user-space virtual memory
	if (remap_pfn_range(vma, start, page, size, vma->vm_page_prot))
		return -EAGAIN; // Return error if remap fails

    	// Set data
    	memcpy(buffer, array, sizeof(array));

    	return 0;
}


static struct file_operations dev_fops = {
	.owner = THIS_MODULE, 
	.open = my_open,
	.mmap = my_map,
};


static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

// Initialization function for the driver
static int __init dev_init(void)
{
	int ret;

	// Allocate memory for the buffer
	buffer = (unsigned char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!buffer) {
		pr_err("Failed to allocate memory\n");
		return -ENOMEM;
	}

	// Register the misc device
	ret = misc_register(&misc);
	if (ret) {
		pr_err("Failed to register misc device\n");
		// Free memory on failure
		kfree(buffer);
		return ret;
	}

	pr_info("Device /dev/%s registered\n", DEVICE_NAME);
	return 0;
}

// Cleanup function for the driver
static void __exit dev_exit(void)
{
	misc_deregister(&misc); // Unregister the misc device
	kfree(buffer);          // Free the allocated memory
	pr_info("Device /dev/%s unregistered\n", DEVICE_NAME);
}


module_init(dev_init);
module_exit(dev_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Willy");
MODULE_DESCRIPTION("Improved mmap example driver");

