// Copyright 2019 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/objects/synthetic-module.h"

#include "src/api/api-inl.h"
#include "src/builtins/accessors.h"
#include "src/objects/js-generator-inl.h"
#include "src/objects/module-inl.h"
#include "src/objects/objects-inl.h"
#include "src/objects/shared-function-info.h"
#include "src/utils/ostreams.h"

namespace v8 {
namespace internal {

// Implements SetSyntheticModuleBinding:
// https://heycam.github.io/webidl/#setsyntheticmoduleexport
void SyntheticModule::SetExport(Isolate* isolate,
                                Handle<SyntheticModule> module,
                                Handle<String> export_name,
                                Handle<Object> export_value) {
  Handle<ObjectHashTable> exports(module->exports(), isolate);
  Handle<Object> export_object(exports->Lookup(export_name), isolate);
  CHECK(export_object->IsCell());
  Handle<Cell> export_cell(Handle<Cell>::cast(export_object));
  // Spec step 2: Set the mutable binding of export_name to export_value
  export_cell->set_value(*export_value);
}

// Implements Synthetic Module Record's ResolveExport concrete method:
// https://heycam.github.io/webidl/#smr-resolveexport
MaybeHandle<Cell> SyntheticModule::ResolveExport(
    Isolate* isolate, Handle<SyntheticModule> module,
    Handle<String> module_specifier, Handle<String> export_name,
    MessageLocation loc, bool must_resolve, ResolveSet* resolve_set) {
  UNREACHABLE();  // TODO(SyntheticModules) implement
}

// Implements Synthetic Module Record's Instantiate concrete method :
// https://heycam.github.io/webidl/#smr-instantiate
bool SyntheticModule::PrepareInstantiate(Isolate* isolate,
                                         Handle<SyntheticModule> module,
                                         v8::Local<v8::Context> context,
                                         v8::Module::ResolveCallback callback) {
  Handle<ObjectHashTable> exports(module->exports(), isolate);
  Handle<FixedArray> export_names(module->export_names(), isolate);
  // Spec step 7: For each export_name in module->export_names...
  for (int i = 0, n = export_names->length(); i < n; ++i) {
    // Spec step 7.1: Create a new mutable binding for export_name.
    // Spec step 7.2: Initialize the new mutable binding to undefined.
    Handle<Cell> cell =
        isolate->factory()->NewCell(isolate->factory()->undefined_value());
    Handle<String> name(String::cast(export_names->get(i)), isolate);
    CHECK(exports->Lookup(name).IsTheHole(isolate));
    exports = ObjectHashTable::Put(exports, name, cell);
  }
  module->set_exports(*exports);
  return true;
}

// Second step of module instantiation.  No real work to do for SyntheticModule
// as there are no imports or indirect exports to resolve;
// just update status.
bool SyntheticModule::FinishInstantiate(Isolate* isolate,
                                        Handle<SyntheticModule> module) {
  module->SetStatus(kInstantiated);
  return true;
}

// Implements Synthetic Module Record's Evaluate concrete method:
// https://heycam.github.io/webidl/#smr-evaluate
MaybeHandle<Object> SyntheticModule::Evaluate(Isolate* isolate,
                                              Handle<SyntheticModule> module) {
  UNREACHABLE();  // TODO(SyntheticModules) implement
}

}  // namespace internal
}  // namespace v8
