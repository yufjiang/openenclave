#include <stdio.h>
#include <stdlib.h>
#include <openenclave.h>
#include <__openenclave/build.h>
#include <__openenclave/sgxdev.h>
#include <__openenclave/sgxtypes.h>
#include "../../host/enclave.h"

#if 1
# define USE_DRIVER
#endif

static OE_SGXDevice* OpenDevice()
{
#ifdef USE_DRIVER
    return __OE_OpenSGXDriver(false);
#else
    return __OE_OpenSGXMeasurer();
#endif
}

static const SGX_EnclaveSettings* GetEnclaveSettings()
{
#ifdef USE_DRIVER
    return NULL;
#else
    static SGX_EnclaveSettings settings;

    memset(&settings, 0, sizeof(SGX_EnclaveSettings));
    settings.debug = 1;
    settings.numHeapPages = 2;
    settings.numStackPages = 1;
    settings.numTCS = 2;

    return &settings;
#endif
}

int main(int argc, const char* argv[])
{
    OE_Result result;
    OE_SGXDevice* dev = NULL;
    OE_Enclave enclave;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s ENCLAVE\n", argv[0]);
        exit(1);
    }

    if (!(dev = OpenDevice()))
        OE_PutErr("__OE_OpenSGXDriver() failed");

    if ((result = __OE_BuildEnclave(
        dev,
        argv[1],
        GetEnclaveSettings(),
        false,
        false,
        &enclave)) != OE_OK)
    {
        OE_PutErr("__OE_AddSegmentPages(): result=%u", result);
    }

    printf("MRENCLAVE=%s\n", OE_SHA256StrOf(&enclave.hash).buf);

    printf("BASEADDR=%016lx\n", enclave.addr);

    for (size_t i = 0; i < enclave.num_tds; i++)
        printf("TCS[%zu]=%016lx\n", i, enclave.tds[i].tcs);

    return 0;
}
