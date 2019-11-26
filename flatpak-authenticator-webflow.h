/*
 * Copyright © 2019 Red Hat, Inc
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *       Alexander Larsson <alexl@redhat.com>
 */

#include "auth-utils.h"

typedef void (*WebflowCallback) (FlatpakAuthenticatorRequest *request,
                                 GHashTable *query,
                                 GError *error);

typedef struct {
  int ref_count;
  gboolean done;
  gboolean started_webflow;

  /* Args */
  FlatpakAuthenticatorRequest *request;
  WebflowCallback callback;
  char *sender;

  /* Results */
  GHashTable *query;
  GError *error;

  char *state;
  SoupServer *server;
} WebflowData;

void webflow_data_unref (WebflowData *data);
WebflowData *webflow_data_ref (WebflowData *data);
void webflow_cancel (WebflowData *data);
WebflowData *webflow_begin (FlatpakAuthenticatorRequest *request,
                            const char *sender,
                            SoupURI *base_uri,
                            const char *uri,
                            WebflowCallback callback);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (WebflowData, webflow_data_unref);
