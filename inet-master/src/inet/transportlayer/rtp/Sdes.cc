//
// Copyright (C) 2001 Matthias Oppitz <Matthias.Oppitz@gmx.de>
// Copyright (C) 2010 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include "inet/transportlayer/rtp/Sdes.h"

#include <string.h>

namespace inet {

namespace rtp {

Register_Class(SdesItem);

SdesItem::SdesItem() : cObject()
{
    _type = SDES_UNDEF;
    _length = 0;
    _content = "";
}

SdesItem::SdesItem(SdesItemType type, const char *content) : cObject()
{
    if (nullptr == content)
        throw cRuntimeError("The content parameter must be a valid pointer.");
    _type = type;
    _content = content;
    // an sdes item requires one byte for the type field,
    // one byte for the length field and bytes for
    // the content string
    _length = _content.length();
}

SdesItem::SdesItem(const SdesItem& sdesItem) : cObject(sdesItem)
{
    copy(sdesItem);
}

SdesItem::~SdesItem()
{
    clean();
}

SdesItem& SdesItem::operator=(const SdesItem& sdesItem)
{
    if (this == &sdesItem)
        return *this;
    clean();
    cObject::operator=(sdesItem);
    copy(sdesItem);
    return *this;
}

void SdesItem::copy(const SdesItem& sdesItem)
{
    _type = sdesItem._type;
    _length = sdesItem._length;
    _content = sdesItem._content;
}

SdesItem *SdesItem::dup() const
{
    return new SdesItem(*this);
}

std::string SdesItem::str() const
{
    std::stringstream out;
    out << "SdesItem=" << _content;
    return out.str();
}

void SdesItem::dump(std::ostream& os) const
{
    os << "SdesItem:" << endl;
    os << "  type = " << _type << endl;
    os << "  content = " << _content << endl;
}

SdesItem::SdesItemType SdesItem::getType() const
{
    return _type;
}

const char *SdesItem::getContent() const
{
    return _content.c_str();
}

int SdesItem::getLengthField() const
{
    // _length contains the length of value without length of Type and Length fields
    return _length;
}

int SdesItem::getSdesTotalLength() const
{
    // bytes needed for this sdes item are
    // one byte for type, one for length
    // and the string
    return _length + 2;
}

//
// SdesChunk
//

Register_Class(SdesChunk);

SdesChunk::SdesChunk(const char *name, uint32_t ssrc) : cArray(name)
{
    _ssrc = ssrc;
    _length = 4;
}

SdesChunk::SdesChunk(const SdesChunk& sdesChunk) : cArray(sdesChunk)
{
    copy(sdesChunk);
}

SdesChunk::~SdesChunk()
{
}

SdesChunk& SdesChunk::operator=(const SdesChunk& sdesChunk)
{
    if (this == &sdesChunk)
        return *this;
    cArray::operator=(sdesChunk);
    copy(sdesChunk);
    return *this;
}

inline void SdesChunk::copy(const SdesChunk& sdesChunk)
{
    _ssrc = sdesChunk._ssrc;
    _length = sdesChunk._length;
}

SdesChunk *SdesChunk::dup() const
{
    return new SdesChunk(*this);
}

std::string SdesChunk::str() const
{
    std::stringstream out;
    out << "SdesChunk.ssrc=" << _ssrc << " items=" << size();
    return out.str();
}

void SdesChunk::dump(std::ostream& os) const
{
    os << "SdesChunk:" << endl;
    os << "  ssrc = " << _ssrc << endl;
    for (int i = 0; i < size(); i++) {
        if (exist(i)) {
            ((const SdesItem *)(get(i)))->dump(os);
        }
    }
}

void SdesChunk::addSDESItem(SdesItem *sdesItem)
{
    for (int i = 0; i < size(); i++) {
        if (exist(i)) {
            SdesItem *compareItem = check_and_cast<SdesItem *>(get(i));
            if (compareItem->getType() == sdesItem->getType()) {
                remove(compareItem);
                _length = _length - compareItem->getSdesTotalLength();
                delete compareItem;
            }
        }
    }

//    sdesItem->setOwner(this);
    add(sdesItem);
    _length += sdesItem->getSdesTotalLength();
}

uint32_t SdesChunk::getSsrc() const
{
    return _ssrc;
}

void SdesChunk::setSsrc(uint32_t ssrc)
{
    _ssrc = ssrc;
}

int SdesChunk::getLength() const
{
    return _length;
}

} // namespace rtp

} // namespace inet

