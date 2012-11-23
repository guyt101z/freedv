//==========================================================================
// Name:            fdmdv2_enum_ports.cpp
// Purpose:         Tools to enumerate hardware ports (other than audio).
// Created:         Oct 2, 2012
// Authors:         David Rowe, David Witten
// 
// License:
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General License version 2.1,
//  as published by the Free Software Foundation.  This program is
//  distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//==========================================================================
#include "fdmdv2_hdw_ports.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// class HdwPort()
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
HdwPort::HdwPort()
{
    m_device_type = -1;
}

//#include <wx/arrimpl.cpp>               // This is a magic incantation which must be done!
//WX_DEFINE_OBJARRAY(ArrayOfPorts);

//-------------------------------------------------------------------------
// OnClose()
//-------------------------------------------------------------------------
HdwPort::~HdwPort()
{
}

//-------------------------------------------------------------------------
// OnClose()
//-------------------------------------------------------------------------
void HdwPort::setType(int portType)
{
    m_device_type = portType;
}

//-------------------------------------------------------------------------
// OnClose()
//-------------------------------------------------------------------------
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

//-------------------------------------------------------------------------
// getSerialPorts()
//-------------------------------------------------------------------------
int HdwPort::getSerialPorts()
{
    int rv = 0;
    return rv;
}

//-------------------------------------------------------------------------
// getUSBPorts()
//-------------------------------------------------------------------------
int HdwPort::getUSBPorts()
{
    int rv = 0;
    return rv;
}

//-------------------------------------------------------------------------
// OnClose()
//-------------------------------------------------------------------------
int HdwPort::getVirtualPorts()
{
    int rv = 0;
    return rv;
}

//-------------------------------------------------------------------------
// getParallelPorts()
//-------------------------------------------------------------------------
int HdwPort::getParallelPorts()
{
    int rv = 0;
    return rv;
}

//-------------------------------------------------------------------------
// getIEEE1394Ports()
//-------------------------------------------------------------------------
int HdwPort::getIEEE1394Ports()
{
    int rv = 0;
    return rv;
}


