#include <fstream>
#include <iostream>
#include <unistd.h>

#include "game.hpp"

namespace Game {
    // reset all data
    void createInitialField() {
        // int seed = 1003940421;
        // srand(seed);
        //
        // // for (int y = 0; y < ROWS; y++) {
        // //     for (int x = 0; x < COLS; x++) {
        // //         if (((y / 10) % 10) % 2 == 0 ^ ((x / 10) % 10) % 2 == 0) {
        // //             baseLayer.at(y).at(x).setChar('.');
        // //             continue;
        // //         } baseLayer.at(y).at(x).setChar('@');
        // //     }
        // // }
        //
        // // for (int y = 1; y < ROWS; y++) {
        // //     for (int x = 1; x < COLS; x++) {
        // //         if (seed ) {
        // //             baseLayer.at(y).at(x).setChar('@');
        // //             continue;
        // //         } baseLayer.at(y).at(x).setChar('.');
        // //     }
        // // }
        //
        // for (int y = 0; y < ROWS; y++) {
        //     for (int x = 0; x < COLS; x++) {
        //         if (rand() % 100 == rand() % 100) {
        //             baseLayer.at(y).at(x).setChar('/');
        //             continue;
        //         } baseLayer.at(y).at(x).setChar('@');
        //     }
        // }
        //
        // for (int y = 0; y < ROWS; y++) {
        //     for (int x = 0; x < COLS; x++) {
        //         if (baseLayer.at(y).at(x).getChar() == '/') {
        //             int offset = rand() % 10 + 3;
        //             for (int xSub = x - offset; xSub < x + offset; xSub++) {
        //                 if (xSub < 0 || xSub >= COLS)
        //                     continue;
        //
        //                 int ySub = sqrt((offset - 1) * (offset - 1) - (xSub - x) * (xSub - x)) + y;
        //
        //                 if (ySub < 0 || ySub >= ROWS)
        //                     continue;
        //
        //                 baseLayer.at(ySub).at(xSub).setChar('.');
        //                 for (int yFill = ySub - 1; yFill > y; yFill--) {
        //                     baseLayer.at(yFill).at(xSub).setChar('.');
        //                 }
        //
        //                 ySub = -sqrt((offset - 1) * (offset - 1) - (xSub - x) * (xSub - x)) + y;
        //
        //                 if (ySub < 0 || ySub >= ROWS)
        //                     continue;
        //
        //                 baseLayer.at(ySub).at(xSub).setChar('.');
        //                 for (int yFill = ySub; yFill <= y; yFill++) {
        //                     baseLayer.at(yFill).at(xSub).setChar('.');
        //                 }
        //             }
        //         }
        //     }
        // }
        //
        // // for (int y = 1; y < ROWS - 1; y++) {
        // //     for (int x = 1; x < COLS - 1; x++) {
        // //         if ((baseLayer.at(y).at(x).getChar() != baseLayer.at(y-1).at(x).getChar() &&
        // //             baseLayer.at(y).at(x).getChar() != baseLayer.at(y+1).at(x).getChar()) ||
        // //             (baseLayer.at(y).at(x).getChar() != baseLayer.at(y).at(x-1).getChar() &&
        // //             baseLayer.at(y).at(x).getChar() != baseLayer.at(y).at(x+1).getChar())) {
        // //             if (baseLayer.at(y).at(x).getChar() == '.')
        // //                 baseLayer.at(y).at(x).setChar('@');
        // //             else baseLayer.at(y).at(x).setChar('.');
        // //             x++;
        // //         }
        // //     }
        // // }
        //
        // baseLayer.at(8).at(2).setChar('$');
        // baseLayer.at(3).at(5).setChar('$');
        //
        // // for (int y = 0; y < ROWS; y++) {
        // //     for (int x = 0; x < COLS; x++) {
        // //         std::cout << baseLayer.at(y).at(x).getChar() << ' ';
        // //     } std::cout << '\n';
        // // }

        constexpr int n = 2;

        for (int x = -n; x <= n; x++) {
            for (int y = -n; y <= n; y++) {
                createEmptyChunk(x, y);
                chunksEdited.emplace_back(x, y);
            }
        }

        const Cell c(0, 3, '$');
        buf[{0, 0}].setCell(c);

        playerX = 0;
        playerY = 0;
        direction = 0;

        // clear previous files
        std::ofstream fileData;
        fileData.open("data.dat",  std::ofstream::out | std::ofstream::trunc);
        fileData.close();

        std::ofstream fileCells;
        fileCells.open("dataCells.dat",  std::ofstream::out | std::ofstream::trunc);
        fileCells.close();

        saveToFile();
    }

    // deserialization chunk data
    void readFromFile() {
        std::fstream fileData("data.dat", std::ios::binary | std::ios::in | std::ios::out);
        std::fstream fileCells("dataCells.dat", std::ios::binary | std::ios::in | std::ios::out);

        fileData.read(reinterpret_cast<char*>(&playerX), sizeof(int));
        fileData.read(reinterpret_cast<char*>(&playerY), sizeof(int));
        fileData.read(reinterpret_cast<char*>(&direction), sizeof(int));
        fileData.read(reinterpret_cast<char*>(&chunkCount), sizeof(int));

        for (int i = 0; i < chunkCount; i++) {
            int chunkX; int chunkY; int offset;

            fileData.read(reinterpret_cast<char*>(&chunkX), sizeof(int));
            fileData.read(reinterpret_cast<char*>(&chunkY), sizeof(int));
            fileData.read(reinterpret_cast<char*>(&offset), sizeof(int));

            chunkAllOffsets[{chunkX, chunkY}] = offset;
            // buf[{chunkX, chunkY}] = loadChunkByOffsetFromFile(&fileCells, chunkX, chunkY, offset);
        }

        fileCells.close();
        fileData.close();
    }

    void loadChunksInPlayerArea() {
        std::fstream file("dataCells.dat", std::ios::binary | std::ios::in | std::ios::out);
        auto [chunkX, chunkY] = getChunkCoords({playerX, playerY});

        int n = 1;
        for (int y = chunkY - n; y < chunkY + n; y++) {
            for (int x = chunkX - n; x < chunkX + n; x++) {
                if (!buf.contains({x, y})) {
                    if (chunkAllOffsets.contains({x, y})) {
                        buf[{x, y}] = loadChunkByOffsetFromFile(&file, x, y, chunkAllOffsets.at({x, y}));
                    }
                    else createEmptyChunk(x, y);
                }
            }
        }
        lightShader();
        file.close();
    }

    Chunk loadChunkByOffsetFromFile(std::fstream* file, int chunkX, int chunkY, int offset) {
        Chunk chunk(chunkX, chunkY, offset);
        file->seekg(offset, std::ios::beg);
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                char ch;
                file->read(&ch, 1);
                chunk.setCell((Cell){x, y, ch});
            }
        } return chunk;
    }

    // serialization chunk data

    // мета игрока / колво чанков / *чанки* / новые чанки
    void saveToFile() {
        std::fstream fileData("data.dat", std::ios::binary | std::ios::in | std::ios::out);
        if (!fileData) {
            fileData.open("data.dat", std::ios::binary | std::ios::out);
            fileData.close();
            fileData.open("data.dat", std::ios::binary | std::ios::in | std::ios::out);
        }

        std::fstream fileCells("dataCells.dat", std::ios::binary | std::ios::in | std::ios::out);
        if (!fileCells) {
            fileCells.open("dataCells.dat", std::ios::binary | std::ios::out);
            fileCells.close();
            fileCells.open("dataCells.dat", std::ios::binary | std::ios::in | std::ios::out);
        }

        fileData.seekp(0, std::ios::beg);

        fileData.write(reinterpret_cast<char*>(&playerX), sizeof(int));
        fileData.write(reinterpret_cast<char*>(&playerY), sizeof(int));
        fileData.write(reinterpret_cast<char*>(&direction), sizeof(int));
        fileData.write(reinterpret_cast<char*>(&chunkCount), sizeof(int));

        fileData.seekp(0, std::ios::end);
        fileCells.seekp(0, std::ios::end);

        for (const auto& chunkCoords : chunksEdited) {
            Chunk* chunk = &buf.at(chunkCoords);
            int offset = buf.at(chunkCoords).getOffset();

            // write chunk if have not before
            if (offset == -1) {
                offset = fileCells.tellp();
                chunk->setOffset(offset);

                const auto& cellsData = chunk->getAllCells();
                for (const auto& cellsRow : cellsData) {
                    for (const auto& cell : cellsRow) {
                        const char ch = cell.getChar();
                        fileCells.write(&ch, sizeof(char));
                    }
                }

                // push new chunk data in fileData with array of chunks offsets
                int x = chunk->getX();
                int y = chunk->getY();
                fileData.write(reinterpret_cast<char*>(&x), sizeof(int));
                fileData.write(reinterpret_cast<char*>(&y), sizeof(int));
                fileData.write(reinterpret_cast<char*>(&offset), sizeof(int));
            }
            // write chunk if already exist in file
            else {
                fileCells.seekp(offset, std::ios::beg);

                const auto& cellsData = chunk->getAllCells();
                for (const auto& cellsRow : cellsData) {
                    for (const auto& cell : cellsRow) {
                        const char ch = cell.getChar();
                        fileCells.write(&ch, sizeof(char));
                    }
                }

                fileCells.seekp(0, std::ios::end);
            }
        }

        fileData.close();
        fileCells.close();

        chunksEdited.clear();
    }

    void createEmptyChunk(int chunkX, int chunkY) {
        Chunk chunk;
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                Cell cell;
                cell.setChar('.');
                cell.setChunkPosition(x, y);
                chunk.setCell(cell);
            }
        }
        chunk.setPosition(chunkX, chunkY);
        buf[{chunkX, chunkY}] = chunk;
        chunkCount++;
    }

    std::pair<int, int> getChunkCoords(const std::pair<int, int>& point) {
        const auto [x, y] = point;

        int chunkX = x / CHUNK_SIZE;
        if (x < 0 && x % CHUNK_SIZE != 0) chunkX--;

        int chunkY = y / CHUNK_SIZE;
        if (y < 0 && y % CHUNK_SIZE != 0) chunkY--;

        return {chunkX, chunkY};
    }

    void fixCoordOutOfChunk(int* coord, int* chunkCoord) {
        if (*coord < 0) {
            *coord += CHUNK_SIZE;
            (*chunkCoord)--;
        }
        else if (*coord >= CHUNK_SIZE) {
            *coord = *coord % CHUNK_SIZE;
            (*chunkCoord)++;
        }
    }

    Cell* getCellFromChunk(const Chunk& chunk, int x, int y) {
        int chunkX = chunk.getX();
        int chunkY = chunk.getY();

        fixCoordOutOfChunk(&x, &chunkX);
        fixCoordOutOfChunk(&y, &chunkY);

        if (buf.contains({chunkX, chunkY})) {
            return buf.at({chunkX, chunkY}).getCell(x, y);
        } return nullptr;
    }
}