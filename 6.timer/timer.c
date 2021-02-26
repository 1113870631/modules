#include <linux/module.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/timer.h>  
#include <linux/of_device.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>


#define LED_OPEN   _IO(type,nr)
#define LED_CLOSE  _IO(type,nr)
#define LED_PERIOD  _IOW(type,nr,size)


struct timerled
{
     dev_t devid;
     char * name;
     int dev_ma;
     int dev_mi;
     int gpio;
     struct class* class;
     struct device* device;
     void * private;
};

struct timerled timerled_dev={
    .name = "timerled",
};

/* 文件操作函数 ************************************************************************ */
int timerled_open (struct inode * inode, struct file *filp)
{
    return 0;
}

static ssize_t timerled_read(struct file *filp, char __user *userbuf,
			     size_t count, loff_t *f_pos)
{
    ssize_t a=0;
    return a;
}

static ssize_t timerled_write(struct file *filp, const char __user *userbuf,
			      size_t count, loff_t *f_pos)
{
  
    ssize_t a=0;
    return a;

}

static int timerled_release(struct inode *inode, struct file *filp)
{
    return 0;
}

int timerled_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
    switch(cmd){
        case:
        break;
        case: 
        break;
        case:
        break;
    }
    return 0;
}


/**********************************************************************************************/

struct file_operations timerled_ops={
    .write=timerled_write,
    .open = timerled_open,
    .release = timerled_release,
    .read    = timerled_read,
    .ioctl   =  timerled_ctl,
    .owner   = THIS_MODULE
};



static int timerledplat_probe(struct platform_device *pdev)
{
    
    int ret =0;
    struct cdev tlmerled_cdev={
        .count = 1,
        .dev = timerled_dev.devid,
        .owner = THIS_MODULE,
    };
    timerled_dev.private = &tlmerled_cdev;
    /* 注册cdev */
    ret = alloc_chrdev_region(&timerled_dev.devid, 0 ,1 ,timerled_dev.name);
        if(ret<0)
        {
            printk("alloc chrdev fail\n");
        }
    cdev_init(&tlmerled_cdev,&timerled_ops);
        
    ret = cdev_add(&tlmerled_cdev,timerled_dev.devid,1);
        if(ret<0)
        {
            printk("cdev_add fail\n");
        }
    timerled_dev.class = class_create(THIS_MODULE,timerled_dev.name);
    timerled_dev.device = device_create(timerled_dev.class,NULL,timerled_dev.devid,NULL,timerled_dev.name);
    /* 获取gpio */
    
    timerled_dev.gpio=  of_get_named_gpio(pdev->dev.of_node,"gpio",0);
        if(timerled_dev.gpio <0)
        {
            printk("get gpio id fail\n");
        }
    ret = gpio_request(timerled_dev.gpio,timerled_dev.name);
        if(ret < 0)
        {
            printk("gpio request fail\n");
        }
    /* 闪烁gpio */
    ret = gpio_direction_output(timerled_dev.gpio,1);
        if(ret<0)
        {
            printk("gpio direction fail\n");
        }
    

    return 0;
}
static int timerledplat_remove(struct platform_device *pdev)
{    
    gpio_set_value(timerled_dev.gpio,0);
    gpio_free(timerled_dev.gpio);
    device_destroy(timerled_dev.class,timerled_dev.devid);
    class_destroy(timerled_dev.class);
    cdev_del(timerled_dev.private);
    unregister_chrdev(timerled_dev.devid,timerled_dev.name);
    return 0;
}

struct of_device_id timerled_of_match[]={

    {.compatible="timerled_plat"},
    {}
};



struct platform_driver timerled_platdriver={
    .probe		= timerledplat_probe,
	.remove		= timerledplat_remove,
	.driver		= {
        .owner  = THIS_MODULE,
		.name	= "timerled_plat",
        .of_match_table = timerled_of_match,
		}
    };
/* 模块入口函数 */
static int __init timerled_init(void)
{

    int ret = 0;
    
    ret = platform_driver_register(&timerled_platdriver);
     if(ret<0)
     {
         printk("plat driver regist fail\n");
     }
    return 0;
}
/* 模块出口函数 */
static void __exit timerled_exit(void)
{
    platform_driver_unregister(&timerled_platdriver);

}





module_init(timerled_init);
module_exit(timerled_exit);
MODULE_LICENSE("GPL");

