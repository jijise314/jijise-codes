#ifndef SORTED_ARRAY_H
#define SORTED_ARRAY_H

/* 정렬된 배열 a[0..*size-1]에 x를 삽입 (배열 용량은 호출자가 보장)
 * 1) 앞에서부터 선형 탐색으로 x 이상인 첫 위치를 찾는다
 * 2) 그 위치부터 끝까지의 데이터를 한 칸씩 뒤로 이동한 뒤 x를 대입한다 */
void sorted_array_insert(int a[], int* size, int x);

/* 정렬된 배열에서 x 한 개를 삭제. 삭제했으면 1, 값이 없으면 0 반환
 * 1) 앞에서부터 선형 탐색 (정렬되어 있으므로 x보다 큰 값을 만나면 중단)
 * 2) 찾은 위치 뒤의 데이터를 한 칸씩 앞으로 이동한다 */
int  sorted_array_delete(int a[], int* size, int x);

#endif
