<?php

class BannerUIController extends EntityDefaultUIController {
  public function hook_menu() {
      $items = parent::hook_menu();
      $items[$this->path]['type'] = MENU_LOCAL_TASK;
      $items[$this->path . '/add']['page callback'] = 'banner_add_page';
      foreach (banner_get_types() as $type) {
        $items['banner/add/' . $type] = array(
          'title' => 'Add Banner',
          'page callback' => 'banner_form_wrapper',
          'page arguments' => array(banner_create(array('type' => $type->type))),
          'access arguments' => array('access content'),
          'type' => MENU_CALLBACK,
          'file' => 'banner.admin.inc',
          'file path' => drupal_get_path('module', 'banner'),
        );
      }
      return $items;
    }
}

function banner_form_wrapper($banner) {
  return drupal_get_form('banner_edit_form', $banner);
}

/**
 * Form callback: create or edit a banner.
 *
 * @param $banner
 *   The banner object to edit or for a create form an empty banner object
 *     with only a banner type defined.
 */
function banner_edit_form($form, &$form_state, $banner) {
  // Add the default field elements.
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($banner->title) ? $banner->title : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );
  

  // Add the field related form elements.
  $form_state['banner'] = $banner;
  field_attach_form('banner', $banner, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save banner'),
    '#submit' => $submit + array('banner_edit_form_submit'),
  );
  
  if (!empty($banner->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete banner'),
      '#suffix' => l(t('Cancel'), 'admin/content/banners'),
      '#submit' => $submit + array('banner_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'banner_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the banner form
 */
function banner_edit_form_validate(&$form, &$form_state) {
  $banner = $form_state['banner'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('banner', $banner, $form, $form_state);
}


/**
 * Form API submit callback for the banner form.
 * 
 * @todo remove hard-coded link
 */
function banner_edit_form_submit(&$form, &$form_state) {
  
  $banner = entity_ui_controller('banner')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the banner and go back to the list of banners
  
  // Add in created and changed times.
  if ($banner->is_new = isset($banner->is_new) ? $banner->is_new : 0){
    $banner->created = time();
  }

  $banner->changed = time();
  
  $banner->save();
  $form_state['redirect'] = 'admin/content/banners';
}

/**
 * Menu Callback to list available box types to add
 */
function banner_add_page() {
  $banner_types = banner_get_types();
  foreach ($banner_types as $type) {
    $content[]= l($type->label, "banner/add/{$type->type}");
  }
  return theme('item_list', array('items' => $content));
}