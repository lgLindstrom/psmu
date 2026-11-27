#include <sdkconfig.h>

#if CONFIG_PSMD_MU_I2C

/* The real MU over I2C. The protocol is open (TBD-28); selecting this backend
 * stops the build instead of inventing one. */

#error "the MU I2C interface is open (TBD-28)"

#endif
