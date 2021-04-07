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
#ifndef __RDK_INFO_H__
#define __RDK_INFO_H__

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
   char *image_name;
   char *stb_name;
   char *branch_name;
   char *version_name;
   char *yocto_version_name;
   char *image_build_time;
   char *jenkins_job_name;
   char *jenkins_build_number;
   char *parse_error;
   bool production_build;
} rdk_version_info_t;

#ifdef __cplusplus
extern "C" {
#endif

unsigned char rdk_version_parse_version(rdk_version_info_t *version_info);
void rdk_version_object_free(rdk_version_info_t *obj);

#ifdef __cplusplus
}
#endif
#endif
