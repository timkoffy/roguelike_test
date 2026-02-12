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

        createChunk(0, 0);
        createChunk(1, 0);

        Cell c(0, 3, '$');
        buf[{0, 0}].setCell(c);
        Cell c1(15, 2, '$');
        buf[{0, 0}].setCell(c1);

        playerX = 0;
        playerY = 0;
        direction = 0;
        // saveToFile();
    }

    void readFromFile() {
        // FILE* f = fopen("data.dat", "rb");
        //
        // // deserialize chunk data
        // // fread(&chunkCount, sizeof(int), 1, f);
        // // fread(chunkIndexes.data(), sizeof(int), chunkCount, f);
        //
        // // for (int i = 0; i < chunkCount; i++) {
        // // }
        //
        // // deserialize field data
        // for (int y = 0; y < ROWS; y++) {
        //     for (int x = 0; x < COLS; x++) {
        //         char ch;
        //         fread(&ch, 1, 1, f);
        //         baseLayer.at(y).at(x) = Cell(x, y, ch, 0);
        //     }
        // }
        //
        //
        // // deserialize player data
        // fread(&playerX, sizeof(int), 1, f);
        // fread(&playerY, sizeof(int), 1, f);
        // fread(&direction, sizeof(int), 1, f);
        //
        // // deserialize entities
        // int entityId, entityX, entityY;
        // while (fread(&entityId, sizeof(int), 1, f) == 1) {
        //     if (entityId == -1) break;
        //     fread(&entityX, sizeof(int), 1, f);
        //     fread(&entityY, sizeof(int), 1, f);
        //     // entities.emplace_back();
        // }
        //
        // fclose(f);
    }

    void saveToFile() {
        // FILE* f = fopen("data.dat", "wb");
        //
        // // serialize field data
        // for (int y = 0; y < ROWS; y++) {
        //     for (int x = 0; x < COLS; x++) {
        //         char ch = baseLayer.at(y).at(x).getChar();
        //         fwrite(&ch, 1, 1, f);
        //     }
        // }
        //
        // // serialize player data
        // fwrite(&playerX, sizeof(int), 1, f);
        // fwrite(&playerY, sizeof(int), 1, f);
        // fwrite(&direction, sizeof(int), 1, f);
        //
        // // serialize entities
        // int entityId, entityX, entityY;
        //
        // // if (entities.empty()) {
        // //     entityId = -1;
        // //     fwrite(&entityId, sizeof(int), 1, f);
        // //     fclose(f);
        // //     return;
        // // }
        // //
        // // for (const auto& entity : entities) {
        // //     entityId = entity.getId();
        // //     entityX = entity.getX();
        // //     entityY = entity.getY();
        // //
        // //     fwrite(&entityId, sizeof(int), 1, f);
        // //     fwrite(&entityX, sizeof(int), 1, f);
        // //     fwrite(&entityY, sizeof(int), 1, f);
        // // }
        //
        // entityId = -1;
        // fwrite(&entityId, sizeof(int), 1, f);
        // fclose(f);
    }

    void createChunk(int chunkX, int chunkY) {
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

    Cell* getCellFromChunk(Chunk& chunk, int x, int y) {
        int chunkX = chunk.getX();
        int chunkY = chunk.getY();

        try {
            return chunk.getCell(x, y);
        } catch (std::out_of_range&) {
            fixCoordOutOfChunk(&x, &chunkX);
            fixCoordOutOfChunk(&y, &chunkY);

            if (buf.contains({chunkX, chunkY})) {
                return buf.at({chunkX, chunkY}).getCell(x, y);
            } return nullptr;
        }
    }
}