#ifndef SYSFS_H
#define SYSFS_H

/* sysfs device selection parsing expression grammar (PEG, look it up):
 *
 * Selection <- FromRoot (Select Filter)* 'FIRST'?
 *              if the FIRST atom is present, this selection will only contain
 *              the first matching device
 *
 * FromRoot <- 'FROM' '(' Root ')'
 * Root <- a C string literal
 *         should always be "/sys/devices" or "/$SYSFS_PATH/devices"
 *
 * Select <- 'SELECT' / 'SELECTUP' / 'AND'
 *           a Select keyword chooses which device(s) to test with the
 *           subsequent filter
 *             SELECT:   select current device and devices in subdirectories
 *             SELECTUP: select devices in superdirectories
 *             AND:      select current device
 *
 * Filters <- SubsystemFilter / SysAttrFilter
 *
 * SubsystemFilter <- 'SUBSYSTEM' '(' Subsystem ')'
 * SysAttrFilter <- 'SYSATTR' '(' SysAttrKey ',' SysAttrValue ')'
 *
 * Subsystem <- a C string literal
 *              examples: "usb", "pci", "tty", etc.
 *              for a complete list, try:
 *                find /sys/devices -name subsystem -printf '%l\n' | rev | cut -f1 -d/ | rev | sort | uniq
 *
 * SysAttrKey <- a C string literal
 *               attribute files in the device directories
 *               examples: "product", "manufacturer"
 *
 * SysAttrValue <- a C string literal
 *                 value to match with the data in an attribute file
 *
 */
#define FROM(x)       " find " x " -maxdepth 0 -print0 "
#define SELECT        " | xargs -0 -I}{ find '}{' "
#define AND           SELECT " -maxdepth 1 "
#define SUBSYSTEM(x)  " -type l -name subsystem -lname \\*/" x " -printf '%h\\0' "
#define SYSATTR(x,y)  " -type f -name " x " -execdir grep -q " y " '{}' \\; -printf '%h\\0' "
#define FIRST         " -quit "

#define SELECTUP      " | xargs -0 -I}{ sh -c 'x=\"}{\"; while [ \"/\" != \"$x\" ]; do dirname -z \"$x\"; x=$(dirname -z \"$x\"); done' " AND

#endif
