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
  watchdog('chooser', $message . drupal_get_destination()['destination'], NULL);
  return  array(
    'registered' => $registered
  );

//  return drupal_json_output($account);
}
/*************************************************************************
 Function:    chooser_storeaccount
 Description: Adds Javascript to the page to instruct the Account Chooser
              to create or update an account record. Information is taken
              from the logged in user.
*/
function chooser_storeaccount() {
  $messages = drupal_get_messages(NULL, TRUE);
  $account = user_uid_optional_load(); 
  if ($account->uid == 0) return "<p>no access</p>";

  $add_photoUrl = "";
  if (isset($account->picture)) {
    $file = file_load($account->picture->fid);
    if (is_object($file)) {
      $raw_url = _HTTPSify(file_create_url($file->uri)); 
      $add_photoUrl = ",photoUrl:\"$raw_url\"";
    }
  }
  $displayName = check_plain($account->name);
  $uiconfig = _get_uiconfig_object();
  $storeobject = "accountchooser.CONFIG.homeUrl=\"?q=user/register\";accountchooser.CONFIG.storeAccount={email:\"$account->mail\",displayName:\"$displayName\"$add_photoUrl};$uiconfig";
  watchdog('chooser', 'Storing account with data %store', array('%store'  => $storeobject),WATCHDOG_NOTICE, $link = NULL);
  drupal_add_js('https://www.accountchooser.com/ac.js', 'external');
  drupal_add_js( $storeobject, 'inline');
  return "<p></p>";
}

/*************************************************************************
 Function:    chooser_branding
 Description: Displays a pure HTML page with no drupal header or footer
              that is read by Account Chooser and then displayed to the user 
              at accountchooser.com
 Called by:   registered in chooser.module as /?q=chooser/branding
 Content:     Can be set in the module configuration pag
 Notes:       CORS headers must be set so that accountchooser.com can access 
              the page. If the page contains anything other than simple HTML, 
              the Account Chooser will refuse to display it.  Also, the page
              must be available via HTTPS or Account Chooser will refuse
	      to display the page. 
*/
function chooser_branding() {
header('Content-Type: text/html');
header("Access-Control-Allow-Origin: *");
header('Access-Control-Allow-Methods: GET, POST, OPTIONS');
header('Access-Control-Max-Age: 86400');
header('Access-Control-Allow-Credentials: true');
	print "<html><body><p>".variable_get('chooser_branding_html')."</p></body></html>"; 
}

