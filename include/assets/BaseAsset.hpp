#pragma once

class BaseAsset {
    public:
        virtual ~BaseAsset() = 0;
};

inline BaseAsset::~BaseAsset() {}