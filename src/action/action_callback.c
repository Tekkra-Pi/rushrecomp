/* Action callback functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* ActionCallback_Init @ 0x02014800 (192 bytes)
 * Initializes action callback system.
 * Args: r0=action */
void ActionCallback_Init(void *action) {
    u32 *data = (u32*)action;
    
    /* Clear callback state */
    u32 i;
    for (i = 0; i < 8; i++) {
        data[8 + i * 2] = 0;    /* +0x20: callback */
        data[9 + i * 2] = 0;    /* +0x24: param */
    }
}

/* ActionCallback_Register @ 0x020148c0 (288 bytes)
 * Registers action callback.
 * Args: r0=action, r1=index, r2=callback, r3=param
 * Returns: 1 if success, 0 otherwise */
s32 ActionCallback_Register(void *action, u32 index, void (*callback)(void*, u32), u32 param) {
    if (index >= 8) return 0;
    
    u32 *data = (u32*)action;
    data[8 + index * 2] = (u32)callback;
    data[9 + index * 2] = param;
    
    return 1;
}

/* ActionCallback_Unregister @ 0x020149e0 (312 bytes)
 * Unregisters action callback.
 * Args: r0=action, r1=index */
void ActionCallback_Unregister(void *action, u32 index) {
    if (index < 8) {
        u32 *data = (u32*)action;
        data[8 + index * 2] = 0;
        data[9 + index * 2] = 0;
    }
}

/* ActionCallback_Invoke @ 0x02014b18
 * Invokes action callback.
 * Args: r0=action, r1=index */
void ActionCallback_Invoke(void *action, u32 index) {
    if (index < 8) {
        u32 *data = (u32*)action;
        void (*callback)(void*, u32) = (void(*)(void*, u32))data[8 + index * 2];
        u32 param = data[9 + index * 2];
        
        if (callback) {
            callback(action, param);
        }
    }
}

/* ActionCallback_InvokeAll @ 0x02014b80
 * Invokes all action callbacks.
 * Args: r0=action */
void ActionCallback_InvokeAll(void *action) {
    u32 *data = (u32*)action;
    u32 i;
    
    for (i = 0; i < 8; i++) {
        void (*callback)(void*, u32) = (void(*)(void*, u32))data[8 + i * 2];
        u32 param = data[9 + i * 2];
        
        if (callback) {
            callback(action, param);
        }
    }
}

/* ActionCallback_GetParam @ 0x02014bf0
 * Returns callback parameter.
 * Args: r0=action, r1=index
 * Returns: parameter */
u32 ActionCallback_GetParam(void *action, u32 index) {
    if (index < 8) {
        u32 *data = (u32*)action;
        return data[9 + index * 2];
    }
    return 0;
}

/* ActionCallback_SetParam @ 0x02014c40
 * Sets callback parameter.
 * Args: r0=action, r1=index, r2=param */
void ActionCallback_SetParam(void *action, u32 index, u32 param) {
    if (index < 8) {
        u32 *data = (u32*)action;
        data[9 + index * 2] = param;
    }
}

/* ActionCallback_IsRegistered @ 0x02014c80
 * Returns whether callback is registered.
 * Args: r0=action, r1=index
 * Returns: 1 if registered, 0 otherwise */
s32 ActionCallback_IsRegistered(void *action, u32 index) {
    if (index < 8) {
        u32 *data = (u32*)action;
        return data[8 + index * 2] != 0;
    }
    return 0;
}
