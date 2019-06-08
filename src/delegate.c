#include <mruby.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <mruby/hash.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/error.h>
#include <mruby/proc.h>
#include <stdlib.h>

#ifndef ELEMENTOF
# define ELEMENTOF(V) (sizeof(V) / sizeof((V)[0]))
#endif

#if MRUBY_RELEASE_NO < 20000
static void
mrb_undef_method_id(mrb_state *mrb, struct RClass *c, mrb_sym mid)
{
  mrb_undef_method(mrb, c, mrb_sym2name(mrb, mid));
}
#endif

#define id_x_setobj_x   mrb_intern_lit(mrb, "__setobj__")
#define C_BASIC_OBJECT  mrb_class_get(mrb, "BasicObject")

static mrb_bool
match_symtab(mrb_sym sym, int num, const mrb_sym tab[])
{
  for (; num > 0; num --, tab ++) {
    if (sym == *tab) { return TRUE; }
  }

  return FALSE;
}

static mrb_value
delegator_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value target;
  mrb_get_args(mrb, "o", &target);
  mrb_funcall_argv(mrb, self, id_x_setobj_x, 1, &target);

  mrb_sym specials[] = {
    mrb_intern_lit(mrb, "__getobj__"),
    mrb_intern_lit(mrb, "__setobj__"),
    mrb_intern_lit(mrb, "method_missing"),
  };

  struct RClass *deleg_c = mrb_class_ptr(mrb_singleton_class(mrb, self));
  struct RClass *c = mrb_class(mrb, target);
  while (c) {
    khash_t(mt) *h = c->mt;
    if (h && kh_size(h) > 0) {
      for (khint_t i = kh_begin(h); i < kh_end(h); i ++) {
        if (kh_exist(h, i)) {
          mrb_method_t m = kh_value(h, i);
          if (!MRB_METHOD_UNDEF_P(m)) {
            mrb_sym mid = kh_key(h, i);
            if (!match_symtab(mid, ELEMENTOF(specials), specials) &&
                mrb_obj_respond_to(mrb, deleg_c, mid)) {
              mrb_undef_method_id(mrb, deleg_c, mid);
            }
          }
        }
      }
    }

    if (c->super == c) { break; }
    c = c->super;
  }

  return self;
}

static mrb_value
delegator_setobj(mrb_state *mrb, mrb_value self)
{
  mrb_value target;
  mrb_get_args(mrb, "o", &target);

  mrb_raise(mrb, E_NOTIMP_ERROR, "need to define `__setobj__' method");

  return mrb_nil_value(); /* not reached here */
}

static mrb_value
delegator_getobj(mrb_state *mrb, mrb_value self)
{
  mrb_get_args(mrb, "");

  mrb_raise(mrb, E_NOTIMP_ERROR, "need to define `__setobj__' method");

  return mrb_nil_value(); /* not reached here */
}

static mrb_value
simple_delegator_setobj(mrb_state *mrb, mrb_value self)
{
  mrb_value target;
  mrb_get_args(mrb, "o", &target);

  mrb_check_type(mrb, self, MRB_TT_ARRAY);
  mrb_ary_modify(mrb, RARRAY(self));
  mrb_ary_set(mrb, self, 0, target);

  return self;
}

static mrb_value
simple_delegator_getobj(mrb_state *mrb, mrb_value self)
{
  mrb_get_args(mrb, "", NULL);

  mrb_check_type(mrb, self, MRB_TT_ARRAY);
  if (RARRAY_LEN(self) != 1) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "not delegated");
  }

  return RARRAY_PTR(self)[0];
}

void
mrb_mruby_delegate_gem_init(mrb_state *mrb)
{
  mrb_intern_lit(mrb, "Delegator");
  mrb_intern_lit(mrb, "SimpleDelegator");
  mrb_intern_lit(mrb, "__setobj__");
  mrb_intern_lit(mrb, "__getobj__");

  struct RClass *delegator = mrb_define_class(mrb, "Delegator", C_BASIC_OBJECT);
  mrb_define_method(mrb, delegator, "initialize", delegator_initialize, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, delegator, "__setobj__", delegator_setobj, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, delegator, "__getobj__", delegator_getobj, MRB_ARGS_NONE());

  struct RClass *simple_delegator = mrb_define_class(mrb, "SimpleDelegator", delegator);
  MRB_SET_INSTANCE_TT(simple_delegator, MRB_TT_ARRAY);
  mrb_define_method(mrb, simple_delegator, "__setobj__", simple_delegator_setobj, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, simple_delegator, "__getobj__", simple_delegator_getobj, MRB_ARGS_NONE());
}

void
mrb_mruby_delegate_gem_final(mrb_state *mrb)
{
}
