#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#define NAME "autocdev"

/*文件操作函数*/
static ssize_t autocdev_read(struct file *file, char __user *buf, size_t count,
		loff_t *offset)
{


    
}


static ssize_t autocdev_write(struct file *file, const char __user *buf,
		size_t count, loff_t *offset)
{
   
   
}


static int autocdev_open(struct inode *inode, struct file *file)
{


  return 0;
}


static int autocdev_release(struct inode *inode, struct file *file)
{
  
    return 0;
}


/*结构体定义*/
typedef struct chrdev
{  
    int major;
    char* name;
}chrdev;

static const struct file_operations automknoddev_fops = {
	.owner		= THIS_MODULE,
	.read		= autocdev_read,
	.write		= autocdev_write,
	.open		= autocdev_open,
	.release	= autocdev_release
};



chrdev autocdev={
    .name=NAME,
    .major=200
};






/*模块入口函数*/
static int __init auto_cdev_init(void)
{
    int ret=0;
    //注册字符设备
    ret=register_chrdev(autocdev.major,autocdev.name,&automknoddev_fops);
     if(ret<0)
     {
         printk("regist chrdev fail");
     }


     printk("init ok!");
}


/*模块出口函数*/
static void __exit auto_cdev_exit(void)
{
    
};

/*模块出入口函数*/
module_init(auto_cdev_init);
module_exit(auto_cdev_exit);
