//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#ifndef __INET_TRANSPORTROUTEOSGVISUALIZER_H
#define __INET_TRANSPORTROUTEOSGVISUALIZER_H

#include "inet/visualizer/osg/base/PathOsgVisualizerBase.h"

namespace inet {

namespace visualizer {

class INET_API TransportRouteOsgVisualizer : public PathOsgVisualizerBase
{
  protected:
    virtual bool isPathStart(cModule *module) const override;
    virtual bool isPathEnd(cModule *module) const override;
    virtual bool isPathElement(cModule *module) const override;
};

} // namespace visualizer

} // namespace inet

#endif

