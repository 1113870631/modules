#include <linux/module.h>
#include <linux/init.h>
#include <linux/of_device.h>
#include <linux/spi/spi.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#define NAME "spi_stmcdev"

/* 函数声明 */
void opt(void);
int spi_testprobe(struct spi_device *spi);
int spi_testremove(struct spi_device *spi);
static int spi_cdev_open(struct inode *inode, struct file *file);
static int spi_cdev_release(struct inode *inode, struct file *file);
static ssize_t spi_cdev_read(struct file *filp, char __user *ubuf,
			     size_t usize, loff_t *off);
static ssize_t spi_cdev_write(struct file *filp, const char __user *ubuf,
			      size_t usize, loff_t *off);
void cs_gpio_init(void);


/* 设备结构体*/
struct spi_dev{
    void * nd;
    dev_t devid;
    int major_id;
    int minor_id;
    char* name;
    struct class* spi_class;
    struct spi_device* spi_device;
    int cs_gpio;
    struct device *  class_device;
};  
struct spi_dev spi_dev={
       .name=NAME,
       .devid=0,
       .major_id=0,
       .minor_id=0,
   };

/*设备树描述参数*/
     struct of_device_id spi_testmatch1[] = {
        { .compatible = "spi_stmconnect" },
        {},
    };

/* cdev字符设备结构体 */
  struct cdev spi_cdev={
        .count=1,
        .owner=THIS_MODULE,
    };
    
/*字符设备操作结构体*/
 struct file_operations spi_cdevfops={
        .write      =spi_cdev_write,
        .open       =spi_cdev_open,
        .read       =spi_cdev_read,
        .release    =spi_cdev_release,
        .owner      =THIS_MODULE,
    };

/* spi驱动结构体 */
    struct spi_driver spidriver={
        .probe=spi_testprobe,
        .remove=spi_testremove,
        .driver={
            .owner=THIS_MODULE,
            .name="spi_stmconnect",
            .of_match_table=spi_testmatch1,
            },
       };
 
/* spi驱动函数 */
     int spi_testprobe(struct spi_device *spi)
    {
     /* 匹配成功后的操作 */
       spi_dev.spi_device=spi;
     /* 注册字符设备 */
       opt();
     /* 获取cs gpio */
       cs_gpio_init();

    }

     int spi_testremove(struct spi_device *spi)
    {  
        /* 释放cs gpio */
        gpio_free(spi_dev.cs_gpio);
        printk("remove\n");
        return 0;

    }
    


/*字符设备文件操作函数*/

    static int spi_cdev_open(struct inode *inode, struct file *file)
{
     
     int ret=0;
     /* 发送一个数据 */

     return 0;
}


static int spi_cdev_release(struct inode *inode, struct file *file)
{
    return 0;
}
static ssize_t spi_cdev_read(struct file *filp, char __user *ubuf,
			     size_t usize, loff_t *off)
{   
    ssize_t a=0;

    return a;
}
static ssize_t spi_cdev_write(struct file *filp, const char __user *ubuf,
			      size_t usize, loff_t *off)
{
    ssize_t a=0;

    return a;
}

void cs_gpio_init(void)
{    
    int ret=0;
    spi_dev.nd=of_find_node_by_path("/mygpio");
     /*获取cs gpio引脚设备树节点*/
    spi_dev.cs_gpio=of_get_gpio(spi_dev.nd , 0);
    if(spi_dev.cs_gpio < 0)
    {
        printk("node get cs gpio fail\n");
    }
     /*申请cs gpio引脚*/
    ret = gpio_request((spi_dev.cs_gpio) , "mygpio");

    if(ret < 0)
    {
        printk("gpio request fail\n");
    } 
    else{
        printk("cs gpio request ok\n");
    } 
    
    ret = gpio_direction_output(spi_dev.cs_gpio , 1); 
    if(ret < 0)
    {
        printk("gpio direction fail\n");
    }  
}


/*匹配执行函数*/

void opt(void)
{ 
  
   int ret=0;
   /*注册设备号*/
   if(spi_dev.major_id)
      {
          ret = register_chrdev_region(spi_dev.devid ,  1, spi_dev.name);
          if(ret<0)
          {
              printk("register_chrdev_region fail\n");
          }
      }
    else{
          printk("alloc reigster\n");
          ret = alloc_chrdev_region(&spi_dev.devid , 0 ,  0,spi_dev.name);
            if(ret<0)
          {
              printk("alloc_chrdev_region fail\n");
          }
        }

    spi_dev.major_id=MAJOR(spi_dev.devid);
    spi_dev.minor_id=MINOR(spi_dev.devid);
    printk("%d\n",spi_dev.major_id);
    printk("%d\n", spi_dev.minor_id);


    /*注册设备*/
    ret = cdev_init(&spi_cdev, &spi_cdevfops);
      if(ret<0)
          {
              printk("cdev_init fail\n");
          }
	ret = cdev_add(&spi_cdev, spi_dev.devid , 1);
      if(ret<0)
          {
              printk("cdev_add fail\n");
          }


    /* 自动创建设备节点 */
    spi_dev.spi_class = class_create(THIS_MODULE, "spi_stmconnect");
    if (IS_ERR(spi_dev.spi_class)) {
		printk("node fail\n");
	}
    spi_dev.class_device = device_create(spi_dev.spi_class, NULL, spi_dev.devid, NULL, "spi_stmconnect");
    printk("probe ->spidev creat ok\n");
}


 



/* 模块入口函数 */
static int __init spi_init(void)
{
    int ret=0;
   /*注册psi驱动*/     
   ret = spi_register_driver(&spidriver);
   if(ret<0)
   {
       printk("spi driver regist fail\n");
   }
   return 0;
}

/* 模块出口函数 */
static void __exit spi_exit(void)
{ 


       
    
        /*删除设备*/
        printk("remove start\n");
        device_destroy(spi_dev.spi_class, spi_dev.devid);
        printk("device remove ok\n");
        /*删除类*/
        printk("remove class\n");
        class_destroy(spi_dev.spi_class);
        printk("remove class ok\n");

        /*删除cdev*/
        printk("remove cdev\n");
        cdev_del(&spi_cdev);
        printk("remove cdev ok\n");
        /*删除设备号*/
        printk("remove devid\n");
        unregister_chrdev_region( spi_cdev.dev, 1);
        printk("remove devid ok\n");
        /*删除spi driver*/
        printk("remove spi\n");
        spi_unregister_driver(&spidriver);
        printk("remove spi ok\n");
        printk("remove ok\n");
        
    
    printk("exit ok\n");
}





module_init(spi_init);
module_exit(spi_exit);
MODULE_LICENSE("GPL");