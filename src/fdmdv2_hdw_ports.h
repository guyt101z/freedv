//==========================================================================
// Name:            fdmdv2_enum_ports.h
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
#ifndef __FDMDV2_HDW_PORTS__
#define __FDMDV2_HDW_PORTS__

#include <string.h>
#include "wx/wx.h"
#include <wx/dynarray.h>
#include "fdmdv2_main.h"

enum
{
    SER_PAR_NONE,
    SER_PAR_ODD,
    SER_PAR_EVEN,
    SER_PAR_MARK,
    SER_PAR_SPACE
};

enum
{
    SER_FLOW_NONE,
    SER_FLOW_XONXOFF,
    SER_FLOW_RTSCTS,
    SER_FLOW_DSRDTR
};

enum
{
    PORT_TYPE_SERIAL,
    PORT_TYPE_USB,
    PORT_TYPE_VIRTUAL,
    PORT_TYPE_PARALLEL,
    PORT_TYPE_IEEE1394,
    PORT_TYPE_UNKNOWN
};

enum
{
    DEVICE_TYPE_SERIAL,
    DEVICE_TYPE_USB,
    DEVICE_TYPE_VIRTUAL,
    DEVICE_TYPE_PARALLEL,
    DEVICE_TYPE_BLUETOOTH,
    DEVICE_TYPE_NETWORK,
    DEVICE_TYPE_IEEE1394,
    DEVICE_TYPE_UNKNOWN
};

class HdwPort;

WX_DECLARE_OBJARRAY(HdwPort, ArrayOfPorts);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
// Class PortList
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=
class HdwPort
{
    public:
        HdwPort();
        ~HdwPort();

    protected:
        wxString    m_name;

    private:
        int     m_device_type;
        int     m_device_emulation;

        void    setType(int portType);
        int     getList();
        int     getSerialPorts();
        int     getUSBPorts();
        int     getVirtualPorts();
        int     getParallelPorts();
        int     getIEEE1394Ports();
};


#endif // __FDMDV2_HDW_PORTS__
