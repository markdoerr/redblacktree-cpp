/* 
 * File:   rbutility.h
 * Author: kurt
 *
 * Created on June 20, 2013, 9:39 AM
 */

#ifndef TREEPRINTER_H
#define TREEPRINTER_H
#include <iosfwd>

class RBPrinter {
    std::ostream& ostr_ref;
public:
    RBPrinter(std::ostream& ostr) : ostr_ref(ostr) {}
    template<typename Key> std::ostream& operator()(Key& k);

};

template<typename Key> inline std::ostream& RBPrinter::operator()(Key& k)
{
    return ostr_ref << k << ' ';
}
#endif	

