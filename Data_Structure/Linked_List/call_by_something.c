
void plus(int *num){
    *num+=3;
}

int main(){
    int a = 1;
    plus(&a);
    printf("%d",a);
}
