//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#ifndef __INET_FIELDSCHUNK_H
#define __INET_FIELDSCHUNK_H

#include "inet/common/packet/chunk/Chunk.h"

namespace inet {

/**
 * This is a base class for classes that represent data using various fields.
 * These classes are most often generated by the omnetpp message compiler.
 */
class INET_API FieldsChunk : public Chunk
{
    friend class Chunk;
    friend class FieldsChunkSerializer;

  protected:
    b chunkLength;
    /**
     * The serialized representation of this chunk or nullptr if not available.
     * When a chunk is serialized, the result is stored here for fast subsequent
     * serializations. Moreover, if a chunk is created by deserialization, then
     * the original bytes are also stored here. The serialized representation
     * is deleted if a chunk is modified.
     */
    mutable const std::vector<uint8_t> *serializedBytes;

  protected:
    /** @name Field accessor functions */
    //@{
    const std::vector<uint8_t> *getSerializedBytes() const { return serializedBytes; }
    void setSerializedBytes(const std::vector<uint8_t> *bytes) const { CHUNK_CHECK_IMPLEMENTATION(B(bytes->size()) == chunkLength); this->serializedBytes = bytes; }
    //@}

    virtual const Ptr<Chunk> peekUnchecked(PeekPredicate predicate, PeekConverter converter, const Iterator& iterator, b length, int flags) const override;

    static const Ptr<Chunk> convertChunk(const std::type_info& typeInfo, const Ptr<Chunk>& chunk, b offset, b length, int flags) {
        return Chunk::convertChunk(typeInfo, chunk, offset, length, flags);
    }

  public:
    /** @name Constructors, destructors and duplication related functions */
    //@{
    FieldsChunk();
    FieldsChunk(const FieldsChunk& other);
    virtual ~FieldsChunk();

    virtual void parsimPack(cCommBuffer *buffer) const override;
    virtual void parsimUnpack(cCommBuffer *buffer) override;
    //@}

    virtual void handleChange() override;

    /** @name Overridden chunk functions */
    //@{
    virtual ChunkType getChunkType() const override { return CT_FIELDS; }

    virtual bool containsSameData(const Chunk& other) const override;

    virtual b getChunkLength() const override { CHUNK_CHECK_IMPLEMENTATION(chunkLength >= b(0)); return chunkLength; }
    virtual void setChunkLength(b chunkLength) { handleChange(); this->chunkLength = chunkLength; }
    virtual void addChunkLength(b chunkLength) { handleChange(); this->chunkLength += chunkLength; }

    virtual std::ostream& printFieldsToStream(std::ostream& stream, int level, int evFlags = 0) const override;
    //@}
};

} // namespace

#endif

