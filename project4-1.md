# GCD 코드에서 복잡도와 BIG O 표기법
이 코드는 GCD를 구하는 코드입니다.  
이 알고리즘은 두 수 중 더 작은 수의 로그(log)값에 비례하는 연산만 수행하면 되는 코드입니다.  
while (b) 반복문 안에서 나머지 연산(a % b)을 수행하는 유클리드 호제법을 사용하였습니다.    
따라서 해당 코드의 전체 시간 복잡도를 BIG O표기법으로 나타내면 O(log(min(|n1|, |n2|))) 입니다. n1과 n2는 입력받은 두 정수 입니다.

---
## 프로파일링 캡쳐본
<img width="1701" height="777" alt="image" src="https://github.com/user-attachments/assets/ada97d58-2a74-4c75-b103-0bb416f4e02f" />
