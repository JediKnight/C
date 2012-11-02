#import <Cocoa/Cocoa.h>

int main(int argc, char **argv)
{
  NSUserNotification *notification = [[NSUserNotification alloc] init];
  [notification setTitle: @"Title"];
  [notification setSubtitle: @"Subtitle"];
  [notification setInformativeText: @"Informative Text"];

  [notification setHasActionButton: YES];
  [notification setActionButtonTitle: @"Action Button"];
  [notification setOtherButtonTitle: @"Other Button"];

  [notification setSoundName: NSUserNotificationDefaultSoundName];

  [notification setDeliveryDate: [NSDate dateWithTimeIntervalSinceNow: 10]];
  [[NSUserNotificationCenter defaultUserNotificationCenter] scheduleNotification: notification];

  return 0;
}
