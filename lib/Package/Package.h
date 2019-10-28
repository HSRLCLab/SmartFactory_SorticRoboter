#ifndef PACKAGE_H
#define PACKAGE_H


struct Package {
    String id = "-1";             ///< PackageID
    String cargo = "-1";          ///< cargo
    String targetDest = "-1";     ///< Target destination of the Package
    String targetReg = "-1";      ///< Target region of the Package
};
#endif