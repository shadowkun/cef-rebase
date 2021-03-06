// Copyright (c) 2013 Marshall A. Greenblatt. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the name Chromium Embedded
// Framework nor the names of its contributors may be used to endorse
// or promote products derived from this software without specific prior
// written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ---------------------------------------------------------------------------
//
// The contents of this file must follow a specific format in order to
// support the CEF translator tool. See the translator.README.txt file in the
// tools directory for more information.
//

#ifndef CEF_INCLUDE_CEF_REQUEST_CONTEXT_H_
#define CEF_INCLUDE_CEF_REQUEST_CONTEXT_H_
#pragma once

#include "include/cef_cookie.h"
#include "include/cef_request_context_handler.h"

class CefSchemeHandlerFactory;

///
// A request context provides request handling for a set of related browser
// or URL request objects. A request context can be specified when creating a
// new browser via the CefBrowserHost static factory methods or when creating a
// new URL request via the CefURLRequest static factory methods. Browser objects
// with different request contexts will never be hosted in the same render
// process. Browser objects with the same request context may or may not be
// hosted in the same render process depending on the process model. Browser
// objects created indirectly via the JavaScript window.open function or
// targeted links will share the same render process and the same request
// context as the source browser. When running in single-process mode there is
// only a single render process (the main process) and so all browsers created
// in single-process mode will share the same request context. This will be the
// first request context passed into a CefBrowserHost static factory method and
// all other request context objects will be ignored.
///
/*--cef(source=library,no_debugct_check)--*/
class CefRequestContext : public virtual CefBase {
 public:
  ///
  // Returns the global context object.
  ///
  /*--cef()--*/
  static CefRefPtr<CefRequestContext> GetGlobalContext();

  ///
  // Creates a new context object with the specified |settings| and optional
  // |handler|.
  ///
  /*--cef(optional_param=handler)--*/
  static CefRefPtr<CefRequestContext> CreateContext(
      const CefRequestContextSettings& settings,
      CefRefPtr<CefRequestContextHandler> handler);

  ///
  // Creates a new context object that shares storage with |other| and uses an
  // optional |handler|.
  ///
  /*--cef(capi_name=create_context_shared,optional_param=handler)--*/
  static CefRefPtr<CefRequestContext> CreateContext(
      CefRefPtr<CefRequestContext> other,
      CefRefPtr<CefRequestContextHandler> handler);

  ///
  // Returns true if this object is pointing to the same context as |that|
  // object.
  ///
  /*--cef()--*/
  virtual bool IsSame(CefRefPtr<CefRequestContext> other) =0;

  ///
  // Returns true if this object is sharing the same storage as |that| object.
  ///
  /*--cef()--*/
  virtual bool IsSharingWith(CefRefPtr<CefRequestContext> other) =0;

  ///
  // Returns true if this object is the global context. The global context is
  // used by default when creating a browser or URL request with a NULL context
  // argument.
  ///
  /*--cef()--*/
  virtual bool IsGlobal() =0;

  ///
  // Returns the handler for this context if any.
  ///
  /*--cef()--*/
  virtual CefRefPtr<CefRequestContextHandler> GetHandler() =0;

  ///
  // Returns the cache path for this object. If empty an "incognito mode"
  // in-memory cache is being used.
  ///
  /*--cef()--*/
  virtual CefString GetCachePath() =0;

  ///
  // Returns the default cookie manager for this object. This will be the global
  // cookie manager if this object is the global request context. Otherwise,
  // this will be the default cookie manager used when this request context does
  // not receive a value via CefRequestContextHandler::GetCookieManager(). If
  // |callback| is non-NULL it will be executed asnychronously on the IO thread
  // after the manager's storage has been initialized.
  ///
  /*--cef(optional_param=callback)--*/
  virtual CefRefPtr<CefCookieManager> GetDefaultCookieManager(
      CefRefPtr<CefCompletionCallback> callback) =0;

  ///
  // Register a scheme handler factory for the specified |scheme_name| and
  // optional |domain_name|. An empty |domain_name| value for a standard scheme
  // will cause the factory to match all domain names. The |domain_name| value
  // will be ignored for non-standard schemes. If |scheme_name| is a built-in
  // scheme and no handler is returned by |factory| then the built-in scheme
  // handler factory will be called. If |scheme_name| is a custom scheme then
  // you must also implement the CefApp::OnRegisterCustomSchemes() method in all
  // processes. This function may be called multiple times to change or remove
  // the factory that matches the specified |scheme_name| and optional
  // |domain_name|. Returns false if an error occurs. This function may be
  // called on any thread in the browser process.
  ///
  /*--cef(optional_param=domain_name,optional_param=factory)--*/
  virtual bool RegisterSchemeHandlerFactory(
      const CefString& scheme_name,
      const CefString& domain_name,
      CefRefPtr<CefSchemeHandlerFactory> factory) =0;

  ///
  // Clear all registered scheme handler factories. Returns false on error. This
  // function may be called on any thread in the browser process.
  ///
  /*--cef()--*/
  virtual bool ClearSchemeHandlerFactories() =0;
};

#endif  // CEF_INCLUDE_CEF_REQUEST_CONTEXT_H_
