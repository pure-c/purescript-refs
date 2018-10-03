#include <purescript.h>

PURS_FFI_FUNC_2(Effect_Ref_new, val, _, {
	return (const purs_any_t *)purs_any_foreign_new(NULL, (void *) val);
});

PURS_FFI_FUNC_2(Effect_Ref_read, _ref, _, {
	return purs_any_get_foreign(_ref)->data;
});

PURS_FFI_FUNC_3(Effect_Ref_modify$, f, _ref, _, {
	purs_foreign_t * ref = (purs_foreign_t *)purs_any_get_foreign(_ref);
	const purs_record_t * res = purs_any_get_record(purs_any_app(f, (const purs_any_t *)ref->data));
	const purs_record_t * state = purs_record_find_by_key(res, "state");
	const purs_record_t * value = purs_record_find_by_key(res, "value");
	ref->data = (purs_any_t *)state->value;
	return value -> value;
});

PURS_FFI_FUNC_3(Effect_Ref_write, val, _ref, _, {
	purs_foreign_t * ref = (purs_foreign_t *)purs_any_get_foreign(_ref);
	ref->data = (purs_any_t *)val;
	return (const purs_any_t *) NULL;
});
