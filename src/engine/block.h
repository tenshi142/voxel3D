#ifndef BLOCK_H
#define BLOCK_H

enum BlockType {
    BlockType_Default = 0,
    BlockType_Grass,
    BlockType_Dirt,
    BlockType_Water,
    BlockType_Stone,
    BlockType_Wood,
    BlockType_Sand,
    BlockType_NumTypes,
};

class Block 
{
    private:
        bool m_active;
        BlockType blockType;

    public:
        Block();
        ~Block();
        bool IsActive();
        void SetActive(bool active);
};

#endif