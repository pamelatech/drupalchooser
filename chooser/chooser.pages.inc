<?php

function chooser_explain() {
	$path = drupal_get_path('module','chooser');
	$message = '<img src="'.$path."/images/accountchooserexample.gif\" align=\"left\"/>This site uses Account Chooser to help you remember your account here, and to reduce how much you have to type. Account Chooser will store a reminder about your account in local storage on this browser, and when you come back, you won't have to type your username - just click your account and that information will be populated for you. This is similar to the \"remember me\" checkboxes on some sites, except that you can use the same reminders across any supported site, both to register and login.<p>";
	$message .= "<p>If your account details are already in the chooser, you'll be asked to select them; if your chooser is empty, we'll ask if we can fill it for you. If you want to register an account that isn't in the list, just click the <b>+Add account</b> button and you'll be taken back to the regular login page.<p>";
	$message .= '<p>Here is an example of the Account Chooser - you can manage the account records in your chooser and learn more, or disable the Chooser altogether by going to <a href="http://accountchooser.com">http://accountchooser.com</a>.</p>';

  return $message;

}

function chooser_userstatus() {
  drupal_add_js('https://www.accountchooser.com/ac.js', 'external');
  $proposed_email = check_plain($_REQUEST['email']);
  $proposed_name = check_plain($_REQUEST['displayName']);
 
  if (empty($proposed_email) && empty($proposed_name)) {
    watchdog('chooser', 'empty userstatus', NULL);
    return NULL;
  }

  $registered = ((int)db_query("SELECT COUNT(*) FROM {users} WHERE name = :name", array(':name' => $proposed_name))->fetchField() > 0);

  if ($registered)
  {  
     $message = "Status: Registered (email $proposed_email name $proposed_name)";
  } else {   
        $message = "Status: Unregistered (email $proposed_email name $proposed_name)";
  }
  watchdog('chooser', $message, NULL);
  return  array(
    'registered' => $registered
  );
//  return drupal_json_output($account);
}

function chooser_storeaccount() {
  $account = user_uid_optional_load(); 
  if ($account->uid == 0) return "<p>no access</p>";
  $file = file_load($account->picture->fid);
 // $url = (is_object($file))?",photoUrl:\"".file_create_url($file->uri)."\"":"";
  $url = (is_object($file))?",photoUrl:\"http://www.gravatar.com/avatar/205e460b479e2e5b48aec07710c08d50\"":"";
  $displayName = check_plain($account->name);
  //$storeobject = "accountchooser.CONFIG.storeAccount={email:\"$account->mail\",displayName:\"$account->name\"};";
  $storeobject = "accountchooser.CONFIG.homeUrl=\"?q=user/register\";accountchooser.CONFIG.storeAccount={email:\"$account->mail\",displayName:\"$displayName\"$url};";
  drupal_add_js('https://www.accountchooser.com/ac.js', 'external');
  drupal_add_js( $storeobject, 'inline');
  return "<p>$storeobject</p>";
}

