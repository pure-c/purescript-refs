#include <purescript.h>

static void release_val(void* tag, void* data) {
	purs_any_t *v = data;
	PURS_ANY_RELEASE(*v);
	free(v);
}

PURS_FFI_FUNC_2(Effect_Ref_new, val, _) {
	PURS_ANY_RETAIN(val);
	purs_any_t* v = purs_malloc(sizeof(purs_any_t));
	memcpy(v, &val, sizeof(purs_any_t));
	const purs_foreign_t* foreign = purs_foreign_new(NULL, v, release_val);
	return purs_any_foreign(foreign);
}

PURS_FFI_FUNC_2(Effect_Ref_read, _ref, _) {
	return *(purs_any_t*)purs_any_get_foreign(_ref)->data;
}

PURS_FFI_FUNC_3(Effect_Ref_modify$, f, _ref, _) {
	const purs_foreign_t* foreign = purs_any_force_foreign(_ref);

	purs_any_t _result = purs_any_app(f, *(purs_any_t*)foreign->data);
	const purs_record_t * x = purs_any_force_record(_result);

	purs_any_t * state = purs_record_find_by_key(x, "state");
	purs_any_t value = *purs_record_find_by_key(x, "value");

	PURS_ANY_RELEASE(*(purs_any_t*)foreign->data);
	PURS_ANY_RETAIN(*state);
	memcpy(foreign->data, state, sizeof(purs_any_t));

	PURS_RC_RELEASE(x);
	PURS_ANY_RELEASE(_result);
	PURS_RC_RELEASE(foreign);

	return value;
}

PURS_FFI_FUNC_3(Effect_Ref_write, val, _ref, _) {
	const purs_foreign_t* foreign = purs_any_force_foreign(_ref);
	PURS_ANY_RELEASE(*(purs_any_t*)foreign->data);
	PURS_ANY_RETAIN(val);
	memcpy(foreign->data, &val, sizeof(purs_any_t));
	PURS_RC_RELEASE(foreign);
	return purs_any_null;
}
