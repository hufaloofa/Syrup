export interface Example {
  name: string;
  code: string;
}

export const examples: Example[] = [
  {
    name: "hello world",
    code: `println("hello from syrup!");`,
  },
  {
    name: "variables & return",
    // test.syr
    code: `let a = 5;
print a;

let a = 3;
print a;

return "at top level";`,
  },
  {
    name: "while loop",
    // test2.syr
    code: `let a = 3;
while (a != 0) {
    print a;
    a = a-1;
}`,
  },
  {
    name: "operators",
    // test3.syr
    code: `let a = 0;
print a;
a++;
print a;
a = 3;
print a;
a++;
print a;
a += 5;
print a;

a *= 3;
print a;

print a == 3;

a /= 27;

print a;

let b = 3;
b *= a*5;
print b;

print (6+3)/3;`,
  },
  {
    name: "fibonacci",
    // test4.syr
    code: `def fib(n) {
  if (n <= 1) return n;
  return fib(n - 2) + fib(n - 1);
}

for (let i = 0; i < 20; i++) {
  println fib(i);
}`,
  },
  {
    name: "closures & scoping",
    // test5.syr
    code: `let a = "global";
{
  def showA() {
    println a;
  }

  showA();
  let a = "block";
  showA();
}`,
  },
];
