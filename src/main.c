/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2017 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include "rdkversion.h"

int main(int argc, char* argv[]) {
   
   // TODO in future add command line options to output other version info

   rdk_version_info_t version_info;
   bzero(&version_info, sizeof(version_info));
   
   if(rdk_version_parse_version(&version_info)) {
      printf("unknown");
   } else {
      printf(version_info.version_name ? version_info.version_name : "null");
   }
   
   rdk_version_object_free(&version_info);
   return(0);
}
