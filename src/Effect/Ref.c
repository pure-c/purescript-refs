#include <purescript.h>

static void Effect_Ref_finalize(void *tag, void *data) {
	purs_any_t any = *((const purs_any_t*) data);
	PURS_ANY_RELEASE(any);
	purs_free(data);
}

PURS_FFI_FUNC_2(Effect_Ref_new, val, _) {
	/* retain value: the ref keeps it alive */
	PURS_ANY_RETAIN(val);

	/* move value onto heap */
	purs_any_t *valheap = purs_new(purs_any_t);
	memcpy(valheap, &val, sizeof(purs_any_t));

	const purs_foreign_t *foreign = purs_foreign_new(
	    NULL,
	    (void *) valheap,
	    Effect_Ref_finalize);
	return purs_any_foreign(foreign);
}

PURS_FFI_FUNC_2(Effect_Ref_read, _ref, _) {
	const purs_foreign_t *foreign = purs_any_unsafe_get_foreign(_ref);
	purs_any_t any = *((const purs_any_t*) foreign->data);
	PURS_ANY_RETAIN(any);
	return any;
}

static purs_str_t state_str = purs_str_static_lazy("state");
static purs_str_t value_str = purs_str_static_lazy("value");

PURS_FFI_FUNC_3(Effect_Ref_modify$, f, _ref, _) {
	const purs_foreign_t *foreign = purs_any_unsafe_get_foreign(_ref);
	purs_any_t currentstate = *((const purs_any_t*) foreign->data);

	const purs_record_t *res = purs_any_force_record(
	    purs_any_app(f, currentstate));

	purs_any_t nextstate = *(purs_record_find_by_key(res, &state_str));
	purs_any_t retvalue = *(purs_record_find_by_key(res, &value_str));

	PURS_ANY_RELEASE(currentstate);
	PURS_ANY_RETAIN(nextstate);

	/* move state onto heap */
	memcpy(foreign->data, &nextstate, sizeof(purs_any_t));

	PURS_RC_RELEASE(res); // release intermediate value
	PURS_RC_RELEASE(res); // additional release due to 'forcing' value

	return retvalue;
}

PURS_FFI_FUNC_3(Effect_Ref_write, nextstate, _ref, _) {
	const purs_foreign_t *foreign = purs_any_unsafe_get_foreign(_ref);
	purs_any_t currentstate = *((const purs_any_t*) foreign->data);

	PURS_ANY_RELEASE(currentstate);
	PURS_ANY_RETAIN(nextstate);

	/* move state onto heap */
	memcpy(foreign->data, &nextstate, sizeof(purs_any_t));

	return purs_any_null;
}
