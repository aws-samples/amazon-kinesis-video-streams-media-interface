#include "sample_common.h"
#include "FHAdv_MD_mpi.h"

#if 0
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

/* Debug printf */
static void frame_print(FH_SINT32 size, ORD_INFO *OrectResult)
{
    int i, j, w, h, area, w2, h2, flag = 0;
    for(i = 0; i < size; i++)
    {
        w = OrectResult[i].x2 - OrectResult[i].x1 + 1;
        h = OrectResult[i].y2 - OrectResult[i].y1 + 1;
        printf("%d size is : %d\n", i, w * h);
    }
    printf("\n");
    for(i = 0; i < size; i++)
    {
        for(j = i + 1; j < size; j++)
        {
            w = OrectResult[i].x2 - OrectResult[i].x1 + 1;
            h = OrectResult[i].y2 - OrectResult[i].y1 + 1;
            w2 = OrectResult[j].x2 - OrectResult[j].x1 + 1;
            h2 = OrectResult[j].y2 - OrectResult[j].y1 + 1;
            if(w * h < w2 * h2)
            {
                printf("Yes\n");
                printf("i:%d, j:%d, area_i:%d, area_j:%d\n", i, j, w * h, w2 * h2);
                printf("\n");
                flag = 1;
                break;
            }
        }
        if (flag == 1)
            break;
    }
}

static void Merge_root(FH_SINT32 *size, ORD_INFO *IrectResult, ORD_INFO *OrectResult)
{
    FH_SINT32 i;
    FH_SINT32 numOfLabels = *size;
    FH_SINT32 numOfRects = 0;

    for(i = 0; i < numOfLabels; i++)
    {
        if (i == IrectResult[i].root)
        {
            OrectResult[numOfRects].x1   = IrectResult[i].x1;
            OrectResult[numOfRects].x2   = IrectResult[i].x2;
            OrectResult[numOfRects].y1   = IrectResult[i].y1;
            OrectResult[numOfRects].y2   = IrectResult[i].y2;
            OrectResult[numOfRects].root = IrectResult[i].root;
            numOfRects++;
        }
    }
    *size = numOfRects;
}

static void InSort_S2L(FH_SINT32 size, ORD_INFO *IOrectResult)
{
    // sort small to large
    FH_UINT32 tmp;
    FH_UINT32 wPrev, hPrev, wCurr, hCurr;
    FH_SINT32 numOfLabels;
    FH_SINT32 i, j;

    numOfLabels = size;
    for (i = 1; i < numOfLabels; i++) 
    {
        for (j = numOfLabels - 1; j >= i; j--) {
            wPrev = (FH_UINT32)(IOrectResult[j - 1].x2 - IOrectResult[j - 1].x1 + 1);
            hPrev = (FH_UINT32)(IOrectResult[j - 1].y2 - IOrectResult[j - 1].y1 + 1);
            wCurr = (FH_UINT32)(IOrectResult[j].x2 - IOrectResult[j].x1 + 1);
            hCurr = (FH_UINT32)(IOrectResult[j].y2 - IOrectResult[j].y1 + 1);
            if (wPrev * hPrev > wCurr * hCurr) {
                // x1
                tmp                  = IOrectResult[j - 1].x1;
                IOrectResult[j - 1].x1 = IOrectResult[j].x1;
                IOrectResult[j].x1     = tmp;

                // x2
                tmp                  = IOrectResult[j - 1].x2;
                IOrectResult[j - 1].x2 = IOrectResult[j].x2;
                IOrectResult[j].x2     = tmp;

                // y2
                tmp                  = IOrectResult[j - 1].y1;
                IOrectResult[j - 1].y1 = IOrectResult[j].y1;
                IOrectResult[j].y1     = tmp;

                // y2
                tmp                  = IOrectResult[j - 1].y2;
                IOrectResult[j - 1].y2 = IOrectResult[j].y2;
                IOrectResult[j].y2     = tmp;

                // root
                tmp                    = IOrectResult[j - 1].root;
                IOrectResult[j - 1].root = IOrectResult[j].root;
                IOrectResult[j].root     = tmp;
            }
        }
    }
    /*sort finished*/
    return;
}

static void Choose_by_Area(FH_SINT32 *size, ORD_INFO *InrectResult, ORD_INFO *OutrectResult)
{
    FH_SINT32 numOfLabels = *size;
    FH_SINT32 numOfRects  = 0;
    FH_UINT32 area_th     = numOfLabels * 15;
    FH_SINT32 i;
    FH_SINT32 rectW;
    FH_SINT32 rectH;
    for (i = 0; i < numOfLabels; i++) {
        rectW = InrectResult[i].x2 - InrectResult[i].x1 + 1;
        rectH = InrectResult[i].y2 - InrectResult[i].y1 + 1;
        if (rectW * rectH > area_th) {
            if (numOfRects >= RECT_MID_RESULT_NUM_MAX)
                break;
            OutrectResult[numOfRects].x1   = InrectResult[i].x1;
            OutrectResult[numOfRects].x2   = InrectResult[i].x2;
            OutrectResult[numOfRects].y1   = InrectResult[i].y1;
            OutrectResult[numOfRects].y2   = InrectResult[i].y2;
            numOfRects++;
        }
    }
    *size = numOfRects;
    return;
}

static void OverLap_Cut(FH_SINT32 *size, ORD_INFO *InrectResult, ORD_INFO *OutrectResult)
{
    FH_SINT32 last;
    FH_SINT32 Icnt;
    FH_SINT32 pickIdx;
    FH_SINT32 xx1, yy1, xx2, yy2;
    FH_SINT32 x1i, x1j, x2i, x2j;
    FH_SINT32 y1i, y1j, y2i, y2j;
    FH_SINT32 w, h, area_j; //area_i;
    FH_SINT32 numOfRects = *size;
    FH_SINT32 i, j, pos, overlap;

    FH_SINT32 *I = fh_osl_malloc(RECT_MID_RESULT_NUM_MAX * sizeof(FH_SINT32));
    for (i = 0; i < numOfRects; i++)
        I[i] = i;

    Icnt    = numOfRects;
    pickIdx = 0;

    while (Icnt != 0) 
    {
        last = numOfRects - 1;

        while (I[last] == -1) {
            last--;
        }

        i             = I[last];  // the highest score
        OutrectResult[pickIdx].x1 = InrectResult[i].x1;
        OutrectResult[pickIdx].x2 = InrectResult[i].x2;
        OutrectResult[pickIdx].y1 = InrectResult[i].y1;
        OutrectResult[pickIdx].y2 = InrectResult[i].y2;

        for (pos = 0; pos <= last - 1; pos++) {
            j = I[pos];
            if (j == -1)
                continue;
            x1i = InrectResult[i].x1;
            x1j = InrectResult[j].x1;
            y1i = InrectResult[i].y1;
            y1j = InrectResult[j].y1;

            x2i = InrectResult[i].x2;
            x2j = InrectResult[j].x2;
            y2i = InrectResult[i].y2;
            y2j = InrectResult[j].y2;

            xx1 = MAX(x1i, x1j);
            yy1 = MAX(y1i, y1j);
            xx2 = MIN(x2i, x2j);
            yy2 = MIN(y2i, y2j);

            w = xx2 - xx1 + 1;
            h = yy2 - yy1 + 1;

            area_j = (x2j - x1j + 1) * (y2j - y1j + 1);

            if (w > 0 && h > 0) {
                if(w * h * 2 > area_j )
                {
                    I[pos] = -1;
                    Icnt--;
                    OutrectResult[pickIdx].x1 = MIN(x1i, x1j);
                    OutrectResult[pickIdx].x2 = MAX(x2i, x2j);
                    OutrectResult[pickIdx].y1 = MIN(y1i, y1j);
                    OutrectResult[pickIdx].y2 = MAX(y2i, y2j);
                    overlap = 1;
                }
                else
                {
                    I[pos] = -1;
                    Icnt--;
                    overlap = 0;
                }
            }
            else
            {
                overlap = 0;
            }
            if (overlap == 1 && pos == last - 1)
            {
                pos = 0;
            }
        }

        I[last] = -1;
        Icnt--;
        pickIdx++;
    }
    *size = pickIdx;
    fh_osl_free(I);
    return;
}

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

    FH_SINT32 pos, posLine;
    FH_SINT32 num;
    FH_SINT32 runLabelW = W_Bg >> 1;

    FH_SINT32 skipFlag;
    FH_SINT32 startPrevRunIdx;
    FH_SINT32 loopCnt = 0;

    // row travers
    FH_UINT16 *currentPosHeadW = fh_osl_malloc((FH_UINT32)runLabelW * sizeof(FH_UINT16));
    FH_UINT16 *lastPosHeadW    = fh_osl_malloc((FH_UINT32)runLabelW * sizeof(FH_UINT16));
    FH_UINT16 *currentPosTailW = fh_osl_malloc((FH_UINT32)runLabelW * sizeof(FH_UINT16));
    FH_UINT16 *lastPosTailW    = fh_osl_malloc((FH_UINT32)runLabelW * sizeof(FH_UINT16));
    FH_UINT32 *currentLabel    = fh_osl_malloc((FH_UINT32)runLabelW * sizeof(FH_UINT32));
    FH_UINT32 *lastLabel       = fh_osl_malloc((FH_UINT32)runLabelW * sizeof(FH_UINT32));
    memset(lastLabel, 0, (FH_UINT32)runLabelW * sizeof(FH_UINT32));
    ORD_INFO *rtable           = fh_osl_malloc((FH_UINT32)(H_Bg * runLabelW) * sizeof(ORD_INFO));
    FH_UINT16 *next            = fh_osl_malloc((FH_UINT32)(H_Bg * runLabelW) * sizeof(FH_UINT16));
    FH_UINT16 *tail            = fh_osl_malloc((FH_UINT32)(H_Bg * runLabelW) * sizeof(FH_UINT16));
    // unsigned short *pRunPosHeadW = fh_osl_malloc((W_Bg << 1)*sizeof(unsigned short));
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

                if (lastPosTailW[prevRunIdx] > currentPosHeadW[currRunIdx] - 5)
                    isPossibleConnected = 1;
                else
                    isPossibleConnected = 0;

                if (isPossibleConnected) 
                {
                    if (lastPosHeadW[prevRunIdx] < currentPosTailW[currRunIdx] + 5) 
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
                            rtable[root].y1 = rtable[root].y1;

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

    fh_osl_free(currentPosHeadW);
    fh_osl_free(lastPosHeadW);
    fh_osl_free(currentPosTailW);
    fh_osl_free(lastPosTailW);
    fh_osl_free(currentLabel);
    fh_osl_free(lastLabel);
    fh_osl_free(next);
    fh_osl_free(tail);

    ORD_INFO *rectResult = fh_osl_malloc((FH_UINT32)MIN(label, RECT_MID_RESULT_NUM_MAX) * sizeof(ORD_INFO));
    ORD_INFO *pick_buf = fh_osl_malloc((FH_UINT32)MIN(label, RECT_MID_RESULT_NUM_MAX) * sizeof(ORD_INFO));
    ORD_INFO *pick = fh_osl_malloc((FH_UINT32)MIN(label, RECT_MID_RESULT_NUM_MAX) * sizeof(ORD_INFO));

    FH_SINT32 numOfRects = label; 
    /* root merge */
    Merge_root(&numOfRects, rtable, rectResult);
    fh_osl_free(rtable);

    /* insert sort from smail to large */
    InSort_S2L(numOfRects, rectResult);

    /* filter overlap */
    OverLap_Cut(&numOfRects, rectResult, pick_buf);
    fh_osl_free(rectResult);

    /* Choose the larger one by area_th */
    Choose_by_Area(&numOfRects, pick_buf, pick);
    fh_osl_free(pick_buf);

    num = 0;
    for (j = 0; j < numOfRects; j++) {
        if (num >= run->rect_num)
            break;
        run->rect[j].u32X      = (FH_UINT32)pick[j].x1;
        run->rect[j].u32Y      = (FH_UINT32)pick[j].y1;
        run->rect[j].u32Width  = (FH_UINT32)(pick[j].x2 - pick[j].x1 + 1);
        run->rect[j].u32Height = (FH_UINT32)(pick[j].y2 - pick[j].y1 + 1);
        num++;
    }
    run->rect_num = num; /*返回实际融合框的个数*/

    fh_osl_free(pick);
}
#endif