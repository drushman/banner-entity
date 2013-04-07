<?php

class Banner extends Entity {
  public $bid;
  public $created;
  public $updated;
  public $uid;
  public $click_number;
  public $view_number;
  public $status;
  public $type;
  
  
  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, $entityType);
    if (!$this->created) $this->created = REQUEST_TIME;
    if (!$this->updated) $this->updated = REQUEST_TIME;
    if (!$this->uid) $this->uid = user_is_anonymous() ? 0 : $GLOBALS['user']->uid;
  }
  
//  public function banner_get_info($banner_id,$type) {
//    switch ($type) {
//      case 'view':
//        $banner = entity_load('banner',$banner_id);
//        return $banner->view_number;
//        break;
//      
//      case 'click':
//        $banner = entity_load('banner',$banner_id);
//        return $banner->click_number;
//        
//      default:
//        return FALSE;
//        break;
//    }
//  }
//  /*
//   * Get all banner by user_id
//   */
//  public function banner_getby_user($uid) {
//    $result = new EntityFieldQuery();
//    $result->entityCondition('banner');
//    $result->fieldConditions('uid', $uid);
//    return $result->execute();    
//  } 
//  /*
//   * Get type of banner
//   */
//  public function gettype(){
//    
//  }
  
  /*
   * 
   */
}
