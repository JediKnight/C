//
//  main.c
//  daemon
//
//  Created by たなか ともあき on 2012/10/05.
//  Copyright (c) 2012年 たなか ともあき. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[])
{
  if (daemon(0, 0) == 0) {
    fprintf(stderr, "daemon");
    exit(1);
  }
  // insert code here...
  printf("Hello, World!\n");
    return 0;
}

