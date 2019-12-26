#include<stdio.h>
#define SYSTICK_RANGE 4294967290	//>>long Int Range		

timer_handler* timer_handler_array[20];		// 20 Software timer Can be created (20 Value choose At Random)
timer_handle_count = 0; 
unsigned int systick = 0;
typedef struct
{
	void (*timer_expire_handler)();
	
	unsigned long int delay_ms;
	unsigned int timer_count;
	unsigned long int ref_systick;
	
}timer_handler;

timer_handler g_timerHandle_1;		//Timer Handler

 timer_handler* TMR_new(unsigned long int delay_ms , void (*handler_fun_ptr)())
{
	timer_handler *dummy_ptr = NULL ;
	if(20 <= timer_handle_count)
	{
		return NULL;		/*Software Timer Creation Is Full*/
	}
	dummy_ptr = (timer_handler *) malloc(sizeof(timer_handler));
	if(NULL==dummy_ptr)
	{
		return NULL;		//timer Handle CanNot be created (Need to Handle Exception In Embedded Case)
	}
	
	dummy_ptr->timer_expire_handler = handler_fun_ptr;
	dummy_ptr->delay_ms = delay_ms;
	dummy_ptr->timer_count = timer_handle_count;
	dummy_ptr->ref_systick = systick;
	
	timer_handler_array[timer_handle_count] = dummy_ptr ; 
	
	timer_handle_count++;
	
	return dummy_ptr; 
}


void destroy_timer(timer_handler * dummy_ptr)
{
	free(dummy_ptr);	//Destroy Means Free Memory
	timer_handle_count --;
}


void ISR_Handler()
{
	if(/*Interrupt Flag*/)
	{
		systick++;		//System Timer 1ms Tick Count
		
		for(int i=0; i<timer_handle_count; i++)
		{
			if( (systick - timer_handler_array[i]->ref_systick) >= timer_handler_array[i]->delay_ms )
			{
				timer_handler_array[i]->timer_expire_handler ;		//Function Pointer Call
			}
		}
	}
	
}


void main()
{
	
	
	
	
}