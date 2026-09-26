/* Gameplay rank helper functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* RankHelper_Init @ 0x02016800 (192 bytes)
 * Initializes rank helper system. */
void RankHelper_Init(void) {
    /* Clear helper state */
    g_rank_helper_score = 0;
    g_rank_helper_time = 0;
    g_rank_helper_rings = 0;
    g_rank_helper_rank = 0;
}

/* RankHelper_Calculate @ 0x020168c0 (288 bytes)
 * Calculates rank based on performance.
 * Args: r0=score, r1=time, r2=rings
 * Returns: rank (0-4) */
u32 RankHelper_Calculate(u32 score, u32 time, u32 rings) {
    u32 rank = 4;  /* Start with D */
    
    /* Score rank */
    if (score >= 1000000) rank = 0;      /* S */
    else if (score >= 500000) rank = 1;  /* A */
    else if (score >= 200000) rank = 2;  /* B */
    else if (score >= 100000) rank = 3;  /* C */
    
    /* Time bonus */
    if (time < 60) rank = 0;  /* S for fast time */
    else if (time < 120 && rank > 1) rank = 1;  /* A */
    
    /* Rings bonus */
    if (rings >= 100 && rank > 2) rank = 2;  /* B */
    
    return rank;
}

/* RankHelper_GetRank @ 0x020169e0 (312 bytes)
 * Returns calculated rank.
 * Args: r0=score, r1=time, r2=rings
 * Returns: rank */
u32 RankHelper_GetRank(u32 score, u32 time, u32 rings) {
    return RankHelper_Calculate(score, time, rings);
}

/* RankHelper_GetRankLetter @ 0x02016b08
 * Returns rank as letter.
 * Args: r0=rank
 * Returns: rank letter ('S', 'A', 'B', 'C', 'D') */
char RankHelper_GetRankLetter(u32 rank) {
    static const char letters[] = "SABCD";
    if (rank > 4) rank = 4;
    return letters[rank];
}

/* RankHelper_GetRankColor @ 0x02016b50
 * Returns rank color.
 * Args: r0=rank
 * Returns: color value */
u32 RankHelper_GetRankColor(u32 rank) {
    static const u32 colors[] = {
        0x7c00,  /* S: Gold */
        0x03e0,  /* A: Green */
        0x001f,  /* B: Blue */
        0x7c1f,  /* C: Purple */
        0x7fff   /* D: White */
    };
    
    if (rank > 4) rank = 4;
    return colors[rank];
}

/* RankHelper_GetRankName @ 0x02016ba0
 * Returns rank name.
 * Args: r0=rank
 * Returns: rank name */
const char* RankHelper_GetRankName(u32 rank) {
    static const char *names[] = {
        "S",
        "A",
        "B",
        "C",
        "D"
    };
    
    if (rank > 4) rank = 4;
    return names[rank];
}

/* RankHelper_GetRankScore @ 0x02016bf0
 * Returns score needed for rank.
 * Args: r0=rank
 * Returns: required score */
u32 RankHelper_GetRankScore(u32 rank) {
    static const u32 scores[] = {
        1000000,  /* S */
        500000,   /* A */
        200000,   /* B */
        100000,   /* C */
        0         /* D */
    };
    
    if (rank > 4) rank = 4;
    return scores[rank];
}

/* RankHelper_GetRankTime @ 0x02016c40
 * Returns time needed for rank.
 * Args: r0=rank
 * Returns: required time in seconds */
u32 RankHelper_GetRankTime(u32 rank) {
    static const u32 times[] = {
        60,   /* S */
        120,  /* A */
        180,  /* B */
        240,  /* C */
        300   /* D */
    };
    
    if (rank > 4) rank = 4;
    return times[rank];
}

/* RankHelper_IsBetterRank @ 0x02016c90
 * Returns whether rank1 is better than rank2.
 * Args: r0=rank1, r1=rank2
 * Returns: 1 if rank1 better, 0 otherwise */
s32 RankHelper_IsBetterRank(u32 rank1, u32 rank2) {
    return rank1 < rank2;
}

/* RankHelper_Update @ 0x02016cd0
 * Updates rank helper with new values.
 * Args: r0=score, r1=time, r2=rings */
void RankHelper_Update(u32 score, u32 time, u32 rings) {
    g_rank_helper_score = score;
    g_rank_helper_time = time;
    g_rank_helper_rings = rings;
    g_rank_helper_rank = RankHelper_Calculate(score, time, rings);
}

/* RankHelper_GetCurrent @ 0x02016d30
 * Returns current rank.
 * Returns: current rank */
u32 RankHelper_GetCurrent(void) {
    return g_rank_helper_rank;
}

/* RankHelper_Reset @ 0x02016d50
 * Resets rank helper system. */
void RankHelper_Reset(void) {
    g_rank_helper_score = 0;
    g_rank_helper_time = 0;
    g_rank_helper_rings = 0;
    g_rank_helper_rank = 0;
}
