#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>

struct gpio_dev
{
   dev_t devid;
   char * name;
   struct cdev cdev;
   int gpio_ma;
   int gpio_mi;
   struct class* class;
   struct device *device;
   int gpio_num;
}; 

struct cdev gpio_cdev={
   .owner=THIS_MODULE,
 };

 struct gpio_dev  gpio_dev={
   
   .name  = "gpio_dev",
   };  

static int newcdev_open(struct inode *inode, struct file *file)
{
   void* nd;
   int gpio_id;
   int ret=0;
   /*获取gpio 设备树节点*/
   nd= of_find_node_by_path("/mygpio");
   if(nd==NULL)
   {
      printk("get nd fail\n");
   }
   else{
      printk("get nd ok\n");
   }

   gpio_id = of_get_named_gpio(nd,"gpio", 0);
    if(gpio_id<0)
    {
       printk("get gpioid fail\n");
    }
    else{
       gpio_dev.gpio_num=gpio_id;
    }

   ret = gpio_request(gpio_id, "test_gpio");
    if(ret<0)
    {
       printk("gpio request fail\n");
    }
    ret = gpio_direction_output(gpio_id, 1);
      if(ret<0)
    {
       printk("gpio set fail\n");
    }

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
struct file_operations gpio_cdvfops={
   .open		=	newcdev_open,
	.release =	newcdev_release,
	.owner   =	THIS_MODULE,
   .read    =  newcdev_read,
   .write   =  newcdev_write
};



/*模块入口函数*/
static int __init newcdev_init(void)
{ 
    int ret=0;
    int major=0;
   
   /*注册设备号*/
   if(major){//给定主设备号
       gpio_dev.devid= MKDEV(major,0);
      ret = register_chrdev_region(gpio_dev.devid, 0 , gpio_dev.name);
      if(ret<0)
      {
         printk("fail regist cdev\n");
         goto failregist;
      }
   } 
   else{//没给定主设备号
         ret = alloc_chrdev_region(&gpio_dev.devid, 0 , 1 , gpio_dev.name);
         gpio_dev.gpio_ma=MAJOR(gpio_dev.devid);
         gpio_dev.gpio_mi=MINOR(gpio_dev.devid);
         if(ret<0)
         {
            printk("fail register cdev\n");
            goto failregist;
         }
   }
   /*注册字符设备*/
     gpio_dev.cdev=gpio_cdev;

    cdev_init(& gpio_dev.cdev,&gpio_cdvfops);
    ret = cdev_add(&gpio_dev.cdev,gpio_dev.devid,1);
    if(ret<0)
    {
       printk("fail cdev add\n");
       goto failcdevadd;
    }
   /*自动创建设备节点*/
   //创建类
   gpio_dev.class = class_create(THIS_MODULE,"gpio_tree_class");
   if (IS_ERR(gpio_dev.class)) {
		  return PTR_ERR(gpio_dev.class);
        }
   //创建设备
   /*struct class *cls, struct device *parent,
			     dev_t devt, void *drvdata,
			     const char *fmt, ...);*/
   gpio_dev.device = device_create(gpio_dev.class, NULL, gpio_dev.devid ,NULL ,"gpio_tree_dev");
    if (IS_ERR(gpio_dev.device)) {
		  return PTR_ERR(gpio_dev.device);
        }

   printk("%d\n",gpio_dev.gpio_ma);
   printk("%d\n",gpio_dev.gpio_mi);
   printk("init ok\n");
    return 0;
   

   failcdevadd:
        unregister_chrdev_region(gpio_dev.devid, 1);
   failregist:
        
        return 0;
}

/*模块出口函数*/

static void __exit newcdev_exit(void)
{
   printk("1close led\n");
   gpio_set_value(gpio_dev.gpio_num , 0 );
   printk("2close ok  free gpio\n");
   gpio_free(gpio_dev.gpio_num);
   printk("3free ok destroy device\n");
   device_destroy(gpio_dev.class,gpio_dev.devid);
   printk("4destroy ok destroy class\n");
   class_destroy(gpio_dev.class);
   printk("5class destroy ok cdev del\n");
   cdev_del(&gpio_cdev);                     //产生warning
   printk("6del cdev ok devid unregister\n");
	unregister_chrdev_region(gpio_dev.devid, 1);
   printk("7ok\n");
   


   printk("rmmod ok\n");             
}

/*模块出入口函数*/
module_init(newcdev_init);
module_exit(newcdev_exit);
MODULE_LICENSE("GPL");