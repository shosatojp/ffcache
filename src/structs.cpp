#include <algorithm>
#include <cmath>

#include "ffcache.hpp"
#include "util.hpp"

//CacheIndexHeader
void CacheIndexHeader::read(std::ifstream& ifs, CacheIndexHeader& header) {
    ifs.read((char*)&header, sizeof(CacheIndexHeader));
    endswap(&header.mVersion);
    endswap(&header.mTimeStamp);
    endswap(&header.mIsDirty);
}

//CacheIndexRecord
void CacheIndexRecord::read(std::ifstream& ifs, CacheIndexRecord& record) {
    ifs.read((char*)&record, sizeof(CacheIndexRecord));
    endswap(&record.mFrecency);
    endswap(&record.mOriginAttrsHash);
    endswap(&record.mExpirationTime);
    endswap(&record.mOnStartTime);
    endswap(&record.mOnStopTime);
    endswap(&record.mFlags);
}

std::string CacheIndexRecord::hash_tostring() const {
    std::stringstream str;
    str << std::hex << std::uppercase;
    for (size_t i = 0; i < kHashSize; i++)
        str << setfill('0') << right << setw(2) << (int)mHash[i];
    return std::string(str.str());
}

//FirefoxMetaData
void FirefoxMetaData::read(std::ifstream& ifs, FirefoxMetaData& fmd) {
    ifs.read((char*)&fmd, sizeof(FirefoxMetaData));
    endswap(&fmd.mVersion);
    endswap(&fmd.mExpirationTime);
    endswap(&fmd.mFetchCount);
    endswap(&fmd.mFlags);
    endswap(&fmd.mFrecency);
    endswap(&fmd.mKeySize);
    endswap(&fmd.mLastFetched);
    endswap(&fmd.mLastModified);
}
