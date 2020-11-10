#ifndef _TASKSHELPER_H_
#define _TASKSHELPER_H_

//void startTask( TaskHandle_t* hndl, taskFunctionPtr* fnc, const char* nme[], uint16_t stk_sz,   ){
void startTask( TaskFunction_t pvTaskCode, const char *constpcName, const uint32_t usStackDepth, void *constpvParameters, UBaseType_t uxPriority, TaskHandle_t *constpvCreatedTask, const BaseType_t xCoreID){
  
  TaskHandle_t xTask = currentGfxTask;

  vTaskSuspendAll();  
  
  if( currentGfxTask != NULL ){
      /* The task is going to be deleted.
      Set the handle to NULL. */
      currentGfxTask = NULL;
  
      /* Delete using the copy of the handle. */
      vTaskDelete( xTask );
  }  
//  if( currentGfxTask != NULL ){
//     vTaskDelete( currentGfxTask );
//     currentGfxTask = NULL;
//   }
  xTaskCreatePinnedToCore(
    pvTaskCode,   /* Task function. */
    constpcName,     /* name of task. */
    usStackDepth,       /* Stack size of task */
    constpvParameters,        /* parameter of the task */
    uxPriority,           /* priority of the task */
    constpvCreatedTask,      /* Task handle to keep track of created task */
    xCoreID);          /* pin task to core 0 */                
      
    xTaskResumeAll();
}

#endif
