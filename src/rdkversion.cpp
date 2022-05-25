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

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include "rdkversion.h"

using namespace std;

#define VERSION_TXT_DIR                  "/"
#define VERSION_TXT_PATH                 "/version.txt"
#define VERSION_TAG_IMAGENAME            "imagename:"
#define VERSION_TAG_BRANCH               "BRANCH="
#define VERSION_TAG_VERSION              "VERSION="
#define VERSION_TAG_YOCTO_VERSION        "YOCTO_VERSION="
#define VERSION_TAG_SPIN                 "SPIN="
#define VERSION_TAG_JENKINS_JOB          "JENKINS_JOB="
#define VERSION_TAG_JENKINS_BUILD_NUMBER "JENKINS_BUILD_NUMBER="
#define VERSION_TAG_BUILD_TIME           "BUILD_TIME="

#ifdef __cplusplus
extern "C" {
#endif

static gboolean rdk_version_value_get(string contents, const char *key, string *value, const char *delimiter);
static gboolean rdk_version_value_contains(string contents, const char *key, const char *value);
static gboolean rdk_version_string_create(string from, char **to);

static gboolean rdk_version_string_create(string from, char **to) {
   size_t str_len = from.size();
   *to = (char *)malloc(str_len + 1);
   if (!*to) {
      return (false);
   }
   strncpy(*to, from.c_str(), str_len);
   (*to)[str_len] = '\0';
   return (true);
}

unsigned char rdk_version_parse_version(rdk_version_info_t *version_info) {
   string image_name_, stb_name_, branch_name_, version_name_, yocto_version_name_, image_build_time_, jenkins_job_name_, jenkins_build_number_, parse_error_;
   const char *version_fn = VERSION_TXT_PATH;
   char *buffer = NULL;
   unsigned char ret_val = 0;

   if(NULL == version_info) {
      return(1);
   }
   bzero(version_info, sizeof(*version_info));

   if(!g_file_test(version_fn, G_FILE_TEST_EXISTS) || !g_file_get_contents(version_fn, &buffer, NULL, NULL)) {
      parse_error_ = VERSION_TXT_PATH " file not found";
      ret_val = 1;
   } else if(NULL == buffer || !strlen(buffer)) {
      parse_error_ = "Empty " VERSION_TXT_PATH " file";
      ret_val = 1;
   } else {
      struct stat statbuf_dir;
      struct stat statbuf_path;

      int rc_dir  = stat(VERSION_TXT_DIR, &statbuf_dir);
      int rc_path = stat(VERSION_TXT_PATH, &statbuf_path);

      // The file is a mount point if the st_dev field returned by stat is different from its directory.
      if(rc_dir != 0 || rc_path != 0 || statbuf_path.st_dev != statbuf_dir.st_dev) {
         if(rc_dir != 0) {
            parse_error_ = "Unable to stat <" VERSION_TXT_DIR ">";
         } else if(rc_path != 0) {
            parse_error_ = "Unable to stat <" VERSION_TXT_PATH ">";
         } else {
            parse_error_ = "version file is mounted";
         }
         ret_val = 1;
      } else {
         string contents((const char *)buffer);
         if(!rdk_version_value_get(contents, VERSION_TAG_IMAGENAME, &image_name_, "\r\n")) {
            parse_error_.append(VERSION_TAG_IMAGENAME);
            parse_error_ += " tag not found, ";
            ret_val = 1;
         }
         if(!rdk_version_value_get(contents, VERSION_TAG_IMAGENAME, &stb_name_, "_\r\n")) {
            parse_error_.append(VERSION_TAG_IMAGENAME);
            parse_error_ += " stbname tag not found, ";
            ret_val = 1;
         }
         if(!rdk_version_value_get(contents, VERSION_TAG_BRANCH, &branch_name_, "\r\n")) {
            parse_error_.append(VERSION_TAG_BRANCH);
            parse_error_ += " tag not found, ";
            ret_val = 1;
         }
         if(!rdk_version_value_get(contents, VERSION_TAG_VERSION, &version_name_, "\r\n")) {
            parse_error_.append(VERSION_TAG_VERSION);
            parse_error_ += " tag not found, ";
            ret_val = 1;
         }
         if(!rdk_version_value_get(contents, VERSION_TAG_YOCTO_VERSION, &yocto_version_name_, "\r\n")) {
            parse_error_.append(VERSION_TAG_YOCTO_VERSION);
            parse_error_ += " tag not found, ";
            ret_val = 1;
         }
         if(!rdk_version_value_get(contents, VERSION_TAG_BUILD_TIME, &image_build_time_, "\r\n")) {
            parse_error_.append(VERSION_TAG_BUILD_TIME);
            parse_error_ += " tag not found, ";
            ret_val = 1;
         }
         if(!rdk_version_value_get(contents, VERSION_TAG_JENKINS_JOB, &jenkins_job_name_, "\r\n")) {
            parse_error_.append(VERSION_TAG_JENKINS_JOB);
            parse_error_ += " tag not found, ";
            ret_val = 1;
         }
         if(!rdk_version_value_get(contents, VERSION_TAG_JENKINS_BUILD_NUMBER, &jenkins_build_number_, "\r\n")) {
            parse_error_.append(VERSION_TAG_JENKINS_BUILD_NUMBER);
            parse_error_ += " tag not found, ";
            ret_val = 1;
         }
         if(rdk_version_value_contains(contents, VERSION_TAG_IMAGENAME, "PROD")) {
            version_info->production_build = 1;
         }
         else {
            version_info->production_build = 0;
         }

         if (!rdk_version_string_create(image_name_, &(version_info->image_name))) {
            parse_error_ += "image_name malloc() error ";
            ret_val = 1;
         }
         if (!rdk_version_string_create(stb_name_, &(version_info->stb_name))) {
            parse_error_ += "stb_name malloc() error ";
            ret_val = 1;
         }
         if (!rdk_version_string_create(branch_name_, &(version_info->branch_name))) {
            parse_error_ += "branch_name malloc() error ";
            ret_val = 1;
         }
         if (!rdk_version_string_create(version_name_, &(version_info->version_name))) {
            parse_error_ += "version_name malloc() error ";
            ret_val = 1;
         }
         if (!rdk_version_string_create(yocto_version_name_, &(version_info->yocto_version_name))) {
            parse_error_ += "yocto_version_name malloc() error ";
            ret_val = 1;
         }
         if (!rdk_version_string_create(image_build_time_, &(version_info->image_build_time))) {
            parse_error_ += "image_build_name malloc() error ";
            ret_val = 1;
         }
         if (!rdk_version_string_create(jenkins_job_name_, &(version_info->jenkins_job_name))) {
            parse_error_ += "jenkins_job_name malloc() error ";
            ret_val = 1;
         }
         if (!rdk_version_string_create(jenkins_build_number_, &(version_info->jenkins_build_number))) {
            parse_error_ += "jenkins_build_number malloc() error ";
            ret_val = 1;
         }
      }
   }

   if (!rdk_version_string_create(parse_error_, &(version_info->parse_error))) {
      ret_val = 1;
   }

   g_free(buffer);
   return(ret_val);
}

static gboolean rdk_version_value_get(string contents, const char *key, string *value, const char *delimiter) {
   size_t key_length = strlen(key);
   size_t delim_pos = 0, prev_pos = 0, key_pos = 0;
   string line;

   while ((delim_pos = contents.find_first_of(delimiter, prev_pos)) != string::npos) {
      line = contents.substr(prev_pos, delim_pos - prev_pos);
      key_pos = line.find(key);
      // The key must match at the beginning of the line, this way if a key exists as substring in another
      // key (like VERSION and YOCTO_VERSION), we'll be able to parse it out correctly.
      if (key_pos == 0) {
         *value = line.substr(key_length, line.length() - key_length);
         return(true);
      }
      prev_pos = delim_pos + 1;
   }
   return(false);
}

static gboolean rdk_version_value_contains(string contents, const char *key, const char *value) {
   size_t key_length = strlen(key);
   size_t startpos   = contents.find(key);
   if(startpos != string::npos) {
      size_t endpos = 0;
      contents = contents.substr(startpos + key_length);
      endpos   = contents.find_first_of("\r\n");
      if(endpos == string::npos) {
         return(false);
      }
      contents = contents.substr(0, endpos);
      endpos   = contents.find(value);
      if(endpos == string::npos) {
         return(false);
      } else {
         return(true);
      }
   }
   return(false);
}

void rdk_version_object_free(rdk_version_info_t *obj) {
   if(obj == NULL) {
      return;
   }
   if(obj->image_name) {
      free(obj->image_name);
   }
   if(obj->stb_name) {
      free(obj->stb_name);
   }
   if (obj->branch_name) {
      free(obj->branch_name);
   }
   if (obj->version_name) {
      free(obj->version_name);
   }
   if (obj->image_build_time) {
      free(obj->image_build_time);
   }
   if (obj->jenkins_job_name) {
      free(obj->jenkins_job_name);
   }
   if (obj->jenkins_build_number) {
      free(obj->jenkins_build_number);
   }
   if (obj->parse_error) {
      free(obj->parse_error);
   }
   bzero(obj, sizeof(*obj));
}
#ifdef __cplusplus
}
#endif

