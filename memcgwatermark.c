/*
 *  linux/mm/memcontrol.c
 *
 *  此代码框架需要加在memconrol中，增加memory子系统水线分级功能
 *  引入usage high 和low水线，以实现异步回收功能
 *
 */

#include <linux/memcontrol.h>
#include <linux/page_counter.h>

//水线结构体定义
//(在 memcontrol.h 中 struct mem_cgroup 里添加)
#pragma region 
struct page_counter WMARK_USAGE
unsigned long WMARK_LOW;
unsigned long WMARK_HIGH;
#pragma endregion

//创建cgroup时，水线初始化，根据cgoup的limit_in_bytes为每个cgoup设定high和low水线
#pragma region 
mem_cgroup.WMARK_LOW = 0.6 * limit_in_bytes //启用用户配置时，= low_rate * limit_in_bytes 
mem_cgroup.WMARK_HIGH = 0.9 * limit_in_bytes //启用用户配置时， = high_rate * limit_in_bytes
//用户自动配置方法，见附件1
#pragma endregion

//以下代码需要加在每个可能申请内存的函数当中(charge) 
#pragma region 
/*
* 函数汇总
* mem_cgroup_newpage_charge();
* mem_cgroup_try_charge_swapin();
* mem_cgroup_cache_charge();
* mem_cgroup_prepare_migration();
*/

//每次申请新内存时时修改水线USAGE的值(try_charge)
    WMARK_USAGE.usage += PAGE_SIZE;
    //若申请失败,内存未真正分配，则恢复计数(cancel)
    if(){
        WMARK_USAGE.usage -= PAGE_SIZE;
    }

//将usage与high比较，到WMARK_HIGH，触发异步回收，调用kswapds

	if (memcg_watermarks.WMARK_USAGE > WMARK_HIGH){
        //mem_cgroup结构体中的info成员可以通过node_id和zone_id
        //找到对应的mem_cgroup_per_zone， reclaim not-shared pages first，
        wake_all_kswapds(order, gfp_mask, ac); 
        
    }
		
        //注：异步回收功能交给kswapds，它包括如何回收、何时睡眠等
        //    在mm/page_alloc.c可以找到它

#pragma endregion

//以下代码需要加在每个可能释放内存的函数当中(uncharge)
#pragma region 
/*
* 函数汇总
* mem_cgroup_uncharge_page();
* mem_cgroup_uncharge_cache_page();
* mem_cgroup_uncharge_swapcache();
* mem_cgroup_uncharge_swap();
* mem_cgroup_end_migration(old,new);
* 以及循环回收过程的函数：
* kswapd_try_to_sleep();
* prepare_kswapd_sleep
*/

//触发WMARK_LOW,主动停止kswapds异步回收
//调用schedule()函数让出cpu控制权进入长期的睡眠状态等待被其它任务wake_up
    if (memcg_watermarks.WMARK_USAGE < WMARK_LOW)
		schedule();

#pragma endregion



