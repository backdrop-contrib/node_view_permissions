<?php

/**
 * Page callback.
 */
function node_view_permissions_settings_page_form($form, &$form_state) {
  $form['description'] = array(
    '#type' => 'item',
    '#markup' => t('Select content types configurable by extra permissions "View own content" and "View any content".'),
  );
  $node_types = array();
  foreach (node_type_get_types() as $type => $info) {
    $node_types[$type] = $info->name;
  }
  $form['node_view_permissions']['node_types'] = array(
    '#type' => 'checkboxes',
    '#options' => $node_types,
    '#default_value' => config_get('node_view_permissions.settings', 'node_types'),
    '#title' => t('Content types'),
  );
  // Add a submit button
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Submit handler.
 */
function node_view_permissions_settings_page_form_submit($form, &$form_state) {
  $config = config('node_view_permissions.settings');
  $config->set('node_types', $form_state['values']['node_types']);
  $config->save();
  backdrop_set_message(t('The configuration options have been saved. Update the !permissions_page in order to select which roles have permission to view the content types you\'ve selected. Any deselected content types keep their role permissions but aren\'t active.', array('!permissions_page' => l(t('permissions page'), 'admin/config/people/permissions'))));

  if (count(module_implements('node_grants'))) {
    node_access_needs_rebuild(TRUE);
    backdrop_set_message(t('You need to !rebuild_permissions.', array('!rebuild_permissions' => l(t('rebuild permissions'), 'admin/reports/status/rebuild'))));
  }
}
