#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>

struct chrdev
{
   dev_t devid;
   char * name;
   struct cdev newcdev;
   int test_ma;
   int test_mi;
   struct class* class;
   struct device *device;
}; 


static int newcdev_open(struct inode *inode, struct file *file)
{

   printk("open\n");
   return 0;
}
static int newcdev_release(struct inode *inode, struct file *file)
{
  
  printk("release\n");
  return 0;
}
ssize_t newcdev_write(struct file *file, const char __user *data,
		       size_t len, loff_t *ppos)
{ 
   ssize_t a=0;
   return a;
}
ssize_t newcdev_read(struct file *file, char __user * buf,
		      size_t len, loff_t * ppos)
{
   ssize_t a=0;
   return a;
}
struct file_operations newtestcdvfops={
   .open		=	newcdev_open,
	.release =	newcdev_release,
	.owner   =	THIS_MODULE,
   .read    =  newcdev_read,
   .write   =  newcdev_write
};

struct cdev newtestcdev={
   .owner=THIS_MODULE,
 };

 struct chrdev  newcdev_test={
   
   .name  = "newtest_cdev",
   };  

/*模块入口函数*/
static int __init newcdev_init(void)
{ 
    int ret=0;
    int major=0;
   
   /*注册设备号*/
   if(major){//给定主设备号
       newcdev_test.devid= MKDEV(major,0);
      ret = register_chrdev_region(newcdev_test.devid, 0 , newcdev_test.name);
      if(ret<0)
      {
         printk("fail regist cdev\n");
         goto failregist;
      }
   } 
   else{//没给定主设备号
         ret = alloc_chrdev_region(&newcdev_test.devid, 0 , 1 , newcdev_test.name);
         newcdev_test.test_ma=MAJOR(newcdev_test.devid);
         newcdev_test.test_mi=MINOR(newcdev_test.devid);
         if(ret<0)
         {
            printk("fail register cdev\n");
            goto failregist;
         }
   }
   /*注册字符设备*/
     newcdev_test.newcdev=newtestcdev;

    cdev_init(&newcdev_test.newcdev,&newtestcdvfops);
    ret = cdev_add(&newcdev_test.newcdev,newcdev_test.devid,1);
    if(ret<0)
    {
       printk("fail cdev add\n");
       goto failcdevadd;
    }
   /*自动创建设备节点*/
   //创建类
   newcdev_test.class = class_create(THIS_MODULE,"newcdev_test");
   if (IS_ERR(newcdev_test.class)) {
		  return PTR_ERR(newcdev_test.class);
        }
   //创建设备
   /*struct class *cls, struct device *parent,
			     dev_t devt, void *drvdata,
			     const char *fmt, ...);*/
   newcdev_test.device = device_create(newcdev_test.class, NULL, newcdev_test.devid ,NULL ,"newcdev_test");
    if (IS_ERR(newcdev_test.device)) {
		  return PTR_ERR(newcdev_test.device);
        }

   printk("%d\n",newcdev_test.test_ma);
   printk("%d\n",newcdev_test.test_mi);
   printk("init ok\n");
    return 0;
   

   failcdevadd:
        unregister_chrdev_region(newcdev_test.devid, 1);
   failregist:
        
        return 0;
}

/*模块出口函数*/

static void __exit newcdev_exit(void)
{

   cdev_del(&newtestcdev);
	unregister_chrdev_region(newcdev_test.devid, 1);
   device_destroy(newcdev_test.class,newcdev_test.devid);
   class_destroy(newcdev_test.class);


   printk("rmmod ok\n");             
}

/*模块出入口函数*/
module_init(newcdev_init);
module_exit(newcdev_exit);
MODULE_LICENSE("GPL");