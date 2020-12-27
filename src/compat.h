#ifndef MRUBY_DELEGATE_COMPAT_H
#define MRUBY_DELEGATE_COMPAT_H 1

#include <mruby.h>

#if MRUBY_RELEASE_NO < 10400
typedef struct RProc *mrb_method_t;

# define MRB_METHOD_UNDEF_P(M) ((M) == NULL)
#endif

#if MRUBY_RELEASE_NO < 20000
static inline void
mrb_undef_method_id(mrb_state *mrb, struct RClass *c, mrb_sym mid)
{
  mrb_undef_method(mrb, c, mrb_sym2name(mrb, mid));
}
#endif

#if MRUBY_RELEASE_NO < 30000
typedef int mrb_mt_foreach_func(mrb_state *, mrb_sym, mrb_method_t, void *);

static inline void
mrb_mt_foreach(mrb_state *mrb, struct RClass *c, mrb_mt_foreach_func *func, void *opaque)
{
  khash_t(mt) *h = c->mt;

  if (h && kh_size(h) > 0) {
    for (khint_t i = kh_begin(h); i < kh_end(h); i++) {
      if (kh_exist(h, i)) {
        mrb_method_t m = kh_value(h, i);
        if (!MRB_METHOD_UNDEF_P(m)) {
          mrb_sym mid = kh_key(h, i);
          if (func(mrb, mid, m, opaque) != 0) {
            return;
          }
        }
      }
    }
  }
}
#else
/* 3.0.0-preview の対応は省略 */
#endif

#endif /* MRUBY_DELEGATE_COMPAT_H */
