<?php

class BannerType extends Entity {
  
  public $id;
  public $type;
  public $label;
  
  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, 'banner_type');
  }
  
  public function save() {
    parent::save();
    field_info_cache_clear();
  }
}

/**
 * UI controller.
 */
class BannerTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage profiles, including fields.';
    return $items;
  }
}

/**
 * Generates the profile type editing form.
 */
function banner_type_form($form, &$form_state, $banner_type, $op = 'edit') {
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $banner_type->label,
    '#description' => t('The human-readable name of this banner type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($banner_type->type) ? $banner_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'banner_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this banner type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save banner type'),
    '#weight' => 40,
  );

  if ($op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete profile type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('banner_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function banner_type_form_submit(&$form, &$form_state) {
  $banner_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $banner_type->save();
  $form_state['redirect'] = 'admin/structure/banner-type';
}

/**
 * Form API submit callback for the delete button.
 */
function banner_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/banner/manage/' . $form_state['banner_type']->type . '/delete';
}