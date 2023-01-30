// parent header
#include <auxml/api/return_status_funcs.h>


void CALLISTO_PYTHON_CALL done_return_status(return_status status)
{
    if(status.error_message == nullptr || status.is_const_message) return;

    delete[] status.error_message;

    status.error_message = nullptr;
}