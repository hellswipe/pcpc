#define SOFTENING 1e-9f

typedef struct { float x, y, z, vx, vy, vz; } Body;

void determisticInitBodies_sequential(Body *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        p[i].x = i + 1;
        p[i].y = i + 1;
        p[i].z = i + 1;
        p[i].vx = i + 1;
        p[i].vy = i + 1;
        p[i].vz = i + 1;
    }
}

void bodyForceSplit_sequential(Body p[], float dt, int own_portion, int start_own_portion, int start, int end, float Fx[], float Fy[], float Fz[])
{
    for (int i = start_own_portion, own_accumulator = 0; i < (start_own_portion + own_portion); i++, own_accumulator++)
    {
        for (int j = start; j < end; j++) // start = inizio nuova porzione da confrontare end = fine porzione da confrontare
        {
            float dx = p[j].x - p[i].x;
            float dy = p[j].y - p[i].y;
            float dz = p[j].z - p[i].z;

            float distSqr = dx * dx + dy * dy + dz * dz + SOFTENING;
            float invDist = 1.0f / sqrt(distSqr);
            float invDist3 = invDist * invDist * invDist;

            Fx[own_accumulator] += dx * invDist3;
            Fy[own_accumulator] += dy * invDist3;
            Fz[own_accumulator] += dz * invDist3;
        }
    }
}

void integratePosition_sequential(Body p[], float dt, int own_portion, int start_own_portion, float Fx[], float Fy[], float Fz[])
{
    for (int i = start_own_portion, j = 0; i < (start_own_portion + own_portion); i++, j++)
    {
        p[i].vx += dt * Fx[j];
        p[i].vy += dt * Fy[j];
        p[i].vz += dt * Fz[j];

        p[i].x += p[i].vx * dt;
        p[i].y += p[i].vy * dt;
        p[i].z += p[i].vz * dt;
    }
}

void printResults_sequential(Body p[], int nBodies , FILE *output_file)
{
    for (int i = 0; i < nBodies; i++)
    {
        char line[100];
        fprintf(output_file, "%f %f %f %f %f %f\n", p[i].x, p[i].y, p[i].z, p[i].vx, p[i].vy, p[i].vz);
    }
}