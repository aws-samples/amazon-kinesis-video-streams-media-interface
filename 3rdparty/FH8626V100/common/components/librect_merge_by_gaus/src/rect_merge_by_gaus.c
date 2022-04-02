#include <stdlib.h>
#include <string.h>
#include "types/type_def.h"
#include "librect_merge_by_gaus/include/rect_merge_by_gaus.h"

#define RECT_MID_RESULT_NUM_MAX 1000
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

typedef struct {
    FH_UINT16 x1;
    FH_UINT16 x2;
    FH_UINT16 y1;
    FH_UINT16 y2;
    FH_UINT16 root;
} ORD_INFO;

void getOrdFromGau(Motion_BGM_RUNTB_RECT *run)
{
    FH_UINT8 *foreground = run->md_ex_result_addr;
    FH_SINT32 confindence = 1; /*for MDEx, fixed to 1*/
    // output: label, rtable
    FH_SINT32 W_Bg = (FH_SINT32)run->horizontal_count; //Bg width after zoomlen ---pixel
    FH_SINT32 H_Bg = (FH_SINT32)run->vertical_count; //Bg height after zoomlen ---pixel
    FH_SINT32          prevRunIdx;
    FH_SINT32          currRunIdx;
    FH_SINT32          isPossibleConnected = 0;
    FH_SINT32          isConnected         = 0;
    FH_SINT32          connectedCnt;
    FH_SINT32          numOfCurrRowRun;
    FH_SINT32          numOfPrevRowRun;
    FH_SINT32          i, j;
    FH_UINT32 root = 0, node = 0;
    FH_UINT32 label = 0;
    FH_UINT32 u, v, k;
    FH_SINT32          numOfLabels;

    FH_SINT32 pos, posLine;
    FH_SINT32 num;
    FH_SINT32 runLabelW = W_Bg >> 1;

    FH_SINT32 rectW;
    FH_SINT32 rectH;

    FH_SINT32 skipFlag;
    FH_SINT32 startPrevRunIdx;
    FH_SINT32 loopCnt = 0;

    // row travers
    FH_UINT16 *currentPosHeadW = malloc((FH_UINT32)runLabelW * sizeof(FH_UINT16));
    FH_UINT16 *lastPosHeadW    = malloc((FH_UINT32)runLabelW * sizeof(FH_UINT16));
    FH_UINT16 *currentPosTailW = malloc((FH_UINT32)runLabelW * sizeof(FH_UINT16));
    FH_UINT16 *lastPosTailW    = malloc((FH_UINT32)runLabelW * sizeof(FH_UINT16));
    FH_UINT32 *currentLabel    = malloc((FH_UINT32)runLabelW * sizeof(FH_UINT32));
    FH_UINT32 *lastLabel       = malloc((FH_UINT32)runLabelW * sizeof(FH_UINT32));
    memset(lastLabel, 0, (FH_UINT32)runLabelW * sizeof(FH_UINT32));
    ORD_INFO *rtable           = malloc((FH_UINT32)(H_Bg * runLabelW) * sizeof(ORD_INFO));
    FH_UINT16 *next            = malloc((FH_UINT32)(H_Bg * runLabelW) * sizeof(FH_UINT16));
    FH_UINT16 *tail            = malloc((FH_UINT32)(H_Bg * runLabelW) * sizeof(FH_UINT16));
    // unsigned short *pRunPosHeadW = malloc((W_Bg << 1)*sizeof(unsigned short));
    num = 0;
    for (i = 0; i < H_Bg; i++) {
        numOfPrevRowRun = num;
        num             = 0;
        startPrevRunIdx = 0;
        posLine         = i * W_Bg;
        for (j = 0; j < W_Bg; j++) {
            pos = posLine + j;
            if (foreground[pos] >= confindence)
            {
                if (j == 0)
                {
                    currentPosHeadW[num] = j;
                }
                else
                {
                    if (foreground[pos - 1] < confindence)
                    {
                        currentPosHeadW[num] = j;
                    }
                }

                if (j == (W_Bg - 1))
                {
                    currentPosTailW[num] = j;
                    num++;
                }
                else
                {
                    if (foreground[pos + 1] < confindence)
                    {
                        currentPosTailW[num] = j;
                        num++;
                    }
                }
            }
        }
        numOfCurrRowRun = num;

        for (currRunIdx = 0; currRunIdx < numOfCurrRowRun; currRunIdx++) {
            connectedCnt = 0;
            skipFlag     = 0;
            for (prevRunIdx = startPrevRunIdx; prevRunIdx < numOfPrevRowRun && skipFlag == 0;
                 prevRunIdx++)
            // for (prevRunIdx=numOfPrevRowRun-1; prevRunIdx >=0; prevRunIdx--)
            {
                if (lastPosTailW[prevRunIdx] >= currentPosTailW[currRunIdx])
                    skipFlag = 1;
                if (lastPosTailW[prevRunIdx] <= currentPosTailW[currRunIdx])
                    startPrevRunIdx = prevRunIdx + 1;
                loopCnt++;

                if (lastPosTailW[prevRunIdx] > currentPosHeadW[currRunIdx] - 2)
                    isPossibleConnected = 1;
                else
                    isPossibleConnected = 0;

                if (isPossibleConnected)
                {
                    if (lastPosHeadW[prevRunIdx] < currentPosTailW[currRunIdx] + 2)
                    {
                        isConnected  = 1;
                        connectedCnt = connectedCnt + 1;

                    }
                    else
                    {
                        isConnected = 0;
                    }

                    if (isConnected)
                    {
                        if (connectedCnt == 1)
                        {
                            currentLabel[currRunIdx] = lastLabel[prevRunIdx];

                            // ord
                            root            = rtable[currentLabel[currRunIdx]].root;
                            rtable[root].y2 = i;
                            rtable[root].x1 = MIN(rtable[root].x1, currentPosHeadW[currRunIdx]);
                            rtable[root].x2 = MAX(rtable[root].x2, currentPosTailW[currRunIdx]);
                        }
                        else
                        {
                            u = rtable[lastLabel[prevRunIdx]].root;
                            v = rtable[currentLabel[currRunIdx]].root;

                            if (u != v) {
                                if (u < v) {
                                    root = u;
                                    node = v;
                                }

                                else if (u > v) {
                                    root = v;
                                    node = u;
                                }

                                k = node;
                                while (k != 65535) {
                                    rtable[k].root = root;
                                    k              = next[k];
                                }

                                next[tail[root]] = node;
                                tail[root]       = tail[node];

                                rtable[root].y1 = MIN(rtable[root].y1, rtable[node].y1);
                                rtable[root].y2 = MAX(rtable[root].y2, rtable[node].y2);
                                rtable[root].x1 = MIN(rtable[root].x1, rtable[node].x1);
                                rtable[root].x2 = MAX(rtable[root].x2, rtable[node].x2);
                            }
                        }
                    }  // end of if(isConnected)
                }      // end of if (isPossibleConnected)
            }          // end of for (prevRunIdx=0; prevRunIdx<numOfPrevRowRun; prevRunIdx++)

            if (connectedCnt == 0) {
                currentLabel[currRunIdx] = label;
                rtable[label].root       = label;

                // ord
                rtable[label].y1 = i;
                rtable[label].y2 = i;
                rtable[label].x1 = currentPosHeadW[currRunIdx];
                rtable[label].x2 = currentPosTailW[currRunIdx];

                next[label] = 65535;
                tail[label] = label;
                label       = label + 1;
            }
        }  // end of for (currRunIdx=0; currRunIdx<numOfCurrRowRun; currRunIdx++)
        memcpy(lastPosHeadW, currentPosHeadW, (FH_UINT32)numOfCurrRowRun * sizeof(FH_UINT16));
        memcpy(lastPosTailW, currentPosTailW, (FH_UINT32)numOfCurrRowRun * sizeof(FH_UINT16));
        memcpy(lastLabel, currentLabel, (FH_UINT32)numOfCurrRowRun * sizeof(FH_UINT32));
    }  // end of for (i=0; i<H_Bg; i++)

    free(currentPosHeadW);
    free(lastPosHeadW);
    free(currentPosTailW);
    free(lastPosTailW);
    free(currentLabel);
    free(lastLabel);
    free(next);
    free(tail);

    numOfLabels          = (FH_SINT32)label;
    FH_SINT32   numOfRects = 0;
    ORD_INFO *rectResult = malloc((FH_UINT32)MIN(numOfLabels, RECT_MID_RESULT_NUM_MAX) * sizeof(ORD_INFO));
    for (i = 0; i < numOfLabels; i++) {
        rectW = rtable[i].x2 - rtable[i].x1 + 1;
        rectH = rtable[i].y2 - rtable[i].y1 + 1;
        if (i == rtable[i].root && rectW * rectH > run->area_th) {
            if (numOfRects >= RECT_MID_RESULT_NUM_MAX)
                break;
            rectResult[numOfRects].x1   = rtable[i].x1;
            rectResult[numOfRects].x2   = rtable[i].x2;
            rectResult[numOfRects].y1   = rtable[i].y1;
            rectResult[numOfRects].y2   = rtable[i].y2;
            rectResult[numOfRects].root = rtable[i].root;
            numOfRects++;
        }
    }
    free(rtable);
    // sort small to large
    FH_UINT32 tmp;
    FH_UINT32 wPrev, hPrev, wCurr, hCurr;
    for (i = 1; i < numOfRects; i++) {
        for (j = numOfRects - 1; j >= i; j--) {
            wPrev = (FH_UINT32)(rectResult[j - 1].x2 - rectResult[j - 1].x1 + 1);
            hPrev = (FH_UINT32)(rectResult[j - 1].y2 - rectResult[j - 1].y1 + 1);
            wCurr = (FH_UINT32)(rectResult[j].x2 - rectResult[j].x1 + 1);
            hCurr = (FH_UINT32)(rectResult[j].y2 - rectResult[j].y1 + 1);
            if (wPrev * hPrev > wCurr * hCurr) {
                // x1
                tmp                  = rectResult[j - 1].x1;
                rectResult[j - 1].x1 = rectResult[j].x1;
                rectResult[j].x1     = tmp;

                // x2
                tmp                  = rectResult[j - 1].x2;
                rectResult[j - 1].x2 = rectResult[j].x2;
                rectResult[j].x2     = tmp;

                // y2
                tmp                  = rectResult[j - 1].y1;
                rectResult[j - 1].y1 = rectResult[j].y1;
                rectResult[j].y1     = tmp;

                // y2
                tmp                  = rectResult[j - 1].y2;
                rectResult[j - 1].y2 = rectResult[j].y2;
                rectResult[j].y2     = tmp;

                // root
                tmp                    = rectResult[j - 1].root;
                rectResult[j - 1].root = rectResult[j].root;
                rectResult[j].root     = tmp;
            }
        }
    }

    FH_SINT32 last;
    FH_SINT32 Icnt;
    FH_SINT32 pickIdx, suppressIdx;
    FH_SINT32 xx1, yy1, xx2, yy2;
    FH_SINT32 x1i, x1j, x2i, x2j;
    FH_SINT32 y1i, y1j, y2i, y2j;
    FH_SINT32 w, h, area_j; //area_i;

    FH_SINT32 *I = malloc(RECT_MID_RESULT_NUM_MAX * sizeof(FH_SINT32));
    for (i = 0; i < numOfRects; i++)
        I[i] = i;

    Icnt    = numOfRects;
    pickIdx = 0;

    FH_SINT32 *pick     = malloc(RECT_MID_RESULT_NUM_MAX * sizeof(FH_SINT32));
    FH_SINT32 *suppress = malloc(RECT_MID_RESULT_NUM_MAX * sizeof(FH_SINT32));
    while (Icnt != 0) {
        last = numOfRects - 1;

        while (I[last] == -1) {
            last--;
        }

        i             = I[last];  // the highest score
        pick[pickIdx] = i;
        pickIdx++;
        suppressIdx           = 0;
        suppress[suppressIdx] = last;
        for (pos = 0; pos <= last - 1; pos++) {
            j = I[pos];
            if (j == -1)
                continue;
            x1i = rectResult[i].x1;
            x1j = rectResult[j].x1;
            y1i = rectResult[i].y1;
            y1j = rectResult[j].y1;

            x2i = rectResult[i].x2;
            x2j = rectResult[j].x2;
            y2i = rectResult[i].y2;
            y2j = rectResult[j].y2;

            xx1 = MAX(x1i, x1j);
            yy1 = MAX(y1i, y1j);
            xx2 = MIN(x2i, x2j);
            yy2 = MIN(y2i, y2j);

            w = xx2 - xx1 + 1;
            h = yy2 - yy1 + 1;

            area_j = (x2j - x1j + 1) * (y2j - y1j + 1);
            //area_i = (x2i - x1i + 1) * (y2i - y1i + 1);

            if (w > 0 && h > 0) {
                if (w * h * 10 > area_j * 7) {
                    suppressIdx++;
                    suppress[suppressIdx] = pos;
                }
            }
        }

        for (k = 0; k <= (FH_UINT32)suppressIdx; k++) {
            I[suppress[k]] = -1;
            Icnt--;
        }
    }
    free(suppress);
    free(I);
    // int debughere;
    numOfRects    = pickIdx;
    num = 0;
    for (j = 0; j < numOfRects; j++) {
        i = pick[j];
        if (num >= run->rect_num)
            break;
        run->rect[j].u32X      = (FH_UINT32)rectResult[i].x1;
        run->rect[j].u32Y      = (FH_UINT32)rectResult[i].y1;
        run->rect[j].u32Width  = (FH_UINT32)(rectResult[i].x2 - rectResult[i].x1 + 1);
        run->rect[j].u32Height = (FH_UINT32)(rectResult[i].y2 - rectResult[i].y1 + 1);
        num++;
    }
    run->rect_num = num; /*返回实际融合框的个数*/
    free(rectResult);
    free(pick);
}
