/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mgos.h"
#include "mgos_gpio.h"
#include "mgos_wifi.h"
#include "mgos_system.h"
#include "mgos_timers.h"

static void loop(void *arg) {
	LOG(LL_INFO, ("Toggled pin"));
	mgos_gpio_toggle(2);
	(void) arg;
}

static void onConnect(int ev, void *ev_data, void *userdata) {
	LOG(LL_INFO, ("Connected to internet"));
	mgos_set_timer(1000, MGOS_TIMER_REPEAT, loop, NULL);
  	(void) ev;
  	(void) ev_data;
  	(void) userdata;
}

enum mgos_app_init_result mgos_app_init(void) {
  LOG(LL_INFO, ("Hi there"));
  mgos_gpio_setup_output(2, false);
  mgos_event_add_handler(MGOS_WIFI_EV_STA_IP_ACQUIRED, onConnect, NULL);
  return MGOS_APP_INIT_SUCCESS;
}
