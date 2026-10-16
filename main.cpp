#include "raylib.h"
#include <vector>
#include <algorithm>

const int W = 800, H = 300;
const int GROUND_Y = 250;

struct Dino {
    float x = 60, y = GROUND_Y;
    float vy = 0;
    bool ducking = false;
    bool onGround() const { return y >= GROUND_Y; }
    Rectangle rect() const {
        if (ducking) return {x, y - 25, 56, 25};
        return {x, y - 44, 40, 44};
    }
};

struct Obstacle {
    Rectangle r;
    bool bird;
};

struct Cloud { float x, y; };

static void Reset(Dino &d, std::vector<Obstacle> &obs, float &speed, float &score, float &spawnTimer) {
    d = Dino();
    obs.clear();
    speed = 300;
    score = 0;
    spawnTimer = 1.0f;
}

int main() {
    InitWindow(W, H, "Dino Offline");
    SetTargetFPS(60);

    Dino dino;
    std::vector<Obstacle> obs;
    std::vector<Cloud> clouds = {{100, 60}, {400, 40}, {650, 80}};
    float speed, score, spawnTimer, hi = 0, groundOff = 0;
    bool gameOver = false;
    Reset(dino, obs, speed, score, spawnTimer);

    const float GRAVITY = 2200, JUMP = -720;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (!gameOver) {
            bool jump = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP);
            dino.ducking = IsKeyDown(KEY_DOWN) && dino.onGround();
            if (jump && dino.onGround()) dino.vy = JUMP;
            if (IsKeyDown(KEY_DOWN) && !dino.onGround()) dino.vy += GRAVITY * 1.5f * dt;

            dino.vy += GRAVITY * dt;
            dino.y += dino.vy * dt;
            if (dino.y > GROUND_Y) { dino.y = GROUND_Y; dino.vy = 0; }

            score += speed * dt * 0.03f;
            speed = std::min(700.0f, speed + 6 * dt);
            groundOff = (float)((int)(groundOff + speed * dt) % 40);

            spawnTimer -= dt;
            if (spawnTimer <= 0) {
                bool bird = score > 100 && GetRandomValue(0, 3) == 0;
                if (bird) {
                    float by = (float)(GetRandomValue(0, 2) == 0 ? GROUND_Y - 25 : GROUND_Y - 60);
                    obs.push_back({{(float)W, by - 20, 40, 20}, true});
                } else {
                    float h = (float)GetRandomValue(30, 55);
                    float w = (float)GetRandomValue(18, 30);
                    obs.push_back({{(float)W, GROUND_Y - h, w, h}, false});
                }
                spawnTimer = (float)GetRandomValue(70, 160) / 100.0f * (300.0f / speed) + 0.4f;
            }

            for (auto &o : obs) o.r.x -= speed * dt;
            obs.erase(std::remove_if(obs.begin(), obs.end(),
                      [](const Obstacle &o) { return o.r.x + o.r.width < 0; }), obs.end());

            Rectangle d = dino.rect();
            Rectangle hb = {d.x + 4, d.y + 4, d.width - 8, d.height - 6};
            for (auto &o : obs)
                if (CheckCollisionRecs(hb, o.r)) gameOver = true;

            if (gameOver && score > hi) hi = score;
        } else if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
            Reset(dino, obs, speed, score, spawnTimer);
            gameOver = false;
        }

        for (auto &c : clouds) {
            c.x -= 30 * dt;
            if (c.x < -60) { c.x = W + 20; c.y = (float)GetRandomValue(20, 100); }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        Color fg = {83, 83, 83, 255};

        for (auto &c : clouds)
            DrawRectangleRounded({c.x, c.y, 60, 16}, 1.0f, 8, LIGHTGRAY);

        DrawLine(0, GROUND_Y + 1, W, GROUND_Y + 1, fg);
        for (int i = -1; i < W / 40 + 1; i++)
            DrawRectangle(i * 40 - (int)groundOff + 10, GROUND_Y + 8, 8, 2, fg);

        Rectangle d = dino.rect();
        DrawRectangleRec(d, fg);
        if (!dino.ducking) {
            DrawRectangle((int)d.x + 20, (int)d.y - 10, 28, 20, fg);
            DrawRectangle((int)d.x + 38, (int)d.y - 6, 5, 5, RAYWHITE);
            DrawRectangle((int)d.x - 8, (int)d.y + 10, 10, 8, fg);
        } else {
            DrawRectangle((int)d.x + 40, (int)d.y - 4, 22, 16, fg);
            DrawRectangle((int)d.x + 54, (int)d.y, 4, 4, RAYWHITE);
        }
        if (dino.onGround() && !gameOver) {
            bool step = ((int)(GetTime() * 10) % 2) == 0;
            DrawRectangle((int)d.x + (step ? 6 : 22), (int)(d.y + d.height), 8, 4, fg);
        }

        for (auto &o : obs) {
            DrawRectangleRec(o.r, fg);
            if (o.bird) {
                bool up = ((int)(GetTime() * 6) % 2) == 0;
                DrawRectangle((int)o.r.x + 10, (int)o.r.y + (up ? -12 : 20), 18, 12, fg);
            } else {
                DrawRectangle((int)o.r.x - 6, (int)o.r.y + 10, 6, 4, fg);
                DrawRectangle((int)(o.r.x + o.r.width), (int)o.r.y + 16, 6, 4, fg);
            }
        }

        DrawText(TextFormat("HI %05d  %05d", (int)hi, (int)score), W - 190, 10, 20, fg);

        if (gameOver) {
            const char *t = "G A M E  O V E R";
            DrawText(t, (W - MeasureText(t, 30)) / 2, 100, 30, fg);
            const char *s = "ESPACIO para reiniciar";
            DrawText(s, (W - MeasureText(s, 18)) / 2, 145, 18, fg);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
