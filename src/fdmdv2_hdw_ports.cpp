//==========================================================================
// Name:            fdmdv2_enum_ports.cpp
// Purpose:         Tools to enumerate hardware ports (other than audio).
// Created:         Oct 2, 2012
// Initial author:  David Witten
// License:
//
//  Copyright (C) 2012 David Witten
//
//  All rights reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License version 2.1,
//  as published by the Free Software Foundation.  This program is
//  distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//==========================================================================
#include "fdmdv2_hdw_ports.h"

HdwPort::HdwPort()
{
    m_device_type = -1;
}

//#include <wx/arrimpl.cpp>               // This is a magic incantation which must be done!
//WX_DEFINE_OBJARRAY(ArrayOfPorts);

HdwPort::~HdwPort()
{
}

void HdwPort::setType(int portType)
{
    m_device_type = portType;
}

int HdwPort::getList()
{
    int rv = 0;

    if(m_device_type != -1)
    {
        switch(m_device_type)
        {
            case PORT_TYPE_SERIAL:
                rv = getSerialPorts();
                break;

            case PORT_TYPE_USB:
                rv = getUSBPorts();
                break;

            case PORT_TYPE_VIRTUAL:
                rv = getVirtualPorts();
                break;

            case PORT_TYPE_PARALLEL:
                rv = getParallelPorts();
                break;

            case PORT_TYPE_IEEE1394:
                rv = getIEEE1394Ports();
                break;

            case PORT_TYPE_UNKNOWN:
            default:
                rv = 0;
                break;
        }
    }
    return rv;
}

int HdwPort::getSerialPorts()
{
    int rv = 0;
    return rv;
}

int HdwPort::getUSBPorts()
{
    int rv = 0;
    return rv;
}

int HdwPort::getVirtualPorts()
{
    int rv = 0;
    return rv;
}

int HdwPort::getParallelPorts()
{
    int rv = 0;
    return rv;
}

int HdwPort::getIEEE1394Ports()
{
    int rv = 0;
    return rv;
}


