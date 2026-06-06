using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Kiosk
{
    public partial class Form1 : Form
    {
        public class Product
        {
            public string Name { get; set; }
            public int Price { get; set; }
            public string Category { get; set; }
        }

        // 1. 장바구니에 담길 아이템 정보를 저장할 클래스
        public class CartItem
        {
            public string Name { get; set; }
            public int Price { get; set; }
            public int Quantity { get; set; }
            public int TotalPrice => Price * Quantity;
        }

        private List<Product> menuList = new List<Product>()
        {
            // 커피
            new Product { Name = "아메리카노", Price = 2000, Category = "Coffee" },
            new Product { Name = "카페라떼", Price = 3000, Category = "Coffee" },
            new Product { Name = "바닐라라떼", Price = 3500, Category = "Coffee" },
            
            // 논커피
            new Product { Name = "초코라떼", Price = 3500, Category = "NonCoffee" },
            new Product { Name = "자몽에이드", Price = 4000, Category = "NonCoffee" },
            new Product { Name = "녹차", Price = 2500, Category = "NonCoffee" },
            
            // 음식
            new Product { Name = "조각 케이크", Price = 5000, Category = "Food" },
            new Product { Name = "허니 브레드", Price = 5500, Category = "Food" },
            new Product { Name = "크루아상", Price = 3000, Category = "Food" }
        };

        private FlowLayoutPanel menuLayoutPanel;
        
        // 2. 장바구니 목록을 관리할 리스트
        private List<CartItem> cartList = new List<CartItem>();

        public Form1()
        {
            InitializeComponent();
            InitializeMenuPanel();
            WireUpEvents();
        }

        private void InitializeMenuPanel()
        {
            menuLayoutPanel = new FlowLayoutPanel();
            menuLayoutPanel.Location = new Point(150, 43);
            menuLayoutPanel.Size = new Size(600, 370);
            menuLayoutPanel.AutoScroll = true;
            menuLayoutPanel.BackColor = Color.LightGray;

            this.Controls.Add(menuLayoutPanel);
        }

        private void WireUpEvents()
        {
            this.NonCoffee.Click += (s, e) => ShowMenu("NonCoffee");
            this.Food.Click += (s, e) => ShowMenu("Food");
            
            // 3. 결제하기 및 처음으로 버튼에 이벤트 연결
            this.Pay.Click += Pay_Click;
            this.Home.Click += Home_Click;
        }

        private void coffee_Click(object sender, EventArgs e)
        {
            ShowMenu("Coffee");
        }

        private void ShowMenu(string category)
        {
            menuLayoutPanel.Controls.Clear();

            foreach (var product in menuList)
            {
                if (product.Category == category)
                {
                    Button btn = new Button();
                    btn.Text = $"{product.Name}\n\n{product.Price}원";
                    btn.Size = new Size(130, 110);
                    btn.Font = new Font("맑은 고딕", 11, FontStyle.Bold);
                    btn.BackColor = Color.White;
                    btn.Margin = new Padding(10);

                    btn.Click += (sender, e) =>
                    {
                        // 4. 장바구니에 이미 같은 메뉴가 있는지 확인 후 수량 증가 또는 새로 추가
                        var existingItem = cartList.FirstOrDefault(item => item.Name == product.Name);
                        if (existingItem != null)
                        {
                            existingItem.Quantity++;
                        }
                        else
                        {
                            cartList.Add(new CartItem { Name = product.Name, Price = product.Price, Quantity = 1 });
                        }

                        MessageBox.Show($"{product.Name}을(를) 담았습니다! (현재 담은 개수: {cartList.First(i => i.Name == product.Name).Quantity}개)", "장바구니 알림");
                    };

                    menuLayoutPanel.Controls.Add(btn);
                }
            }
        }

        // 5. 결제하기 버튼 로직
        private void Pay_Click(object sender, EventArgs e)
        {
            if (cartList.Count == 0)
            {
                MessageBox.Show("장바구니가 비어 있습니다. 메뉴를 먼저 선택해 주세요!", "알림", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            // 하단에 정의한 결제 창(PaymentForm)을 띄움
            using (PaymentForm paymentForm = new PaymentForm(cartList))
            {
                // 결제창에서 성공적으로 결제가 완료(OK)되면 장바구니를 비움
                if (paymentForm.ShowDialog() == DialogResult.OK)
                {
                    ResetKiosk();
                }
            }
        }

        // 6. 처음으로 버튼 로직
        private void Home_Click(object sender, EventArgs e)
        {
            if (cartList.Count > 0)
            {
                var result = MessageBox.Show("담겨있는 모든 메뉴가 삭제됩니다. 처음으로 돌아가시겠습니까?", "확인", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                if (result == DialogResult.No) return;
            }

            ResetKiosk();
            MessageBox.Show("초기화되었습니다. 처음 화면으로 돌아갑니다.", "알림");
        }

        // 키오스크 상태(장바구니 및 메뉴판 화면)를 완전히 초기화하는 메서드
        private void ResetKiosk()
        {
            cartList.Clear();
            menuLayoutPanel.Controls.Clear();
        }
    }

    // 7. 결제 수단과 담은 목록을 띄워줄 결제용 별도 창 클래스
    public class PaymentForm : Form
    {
        public PaymentForm(List<Form1.CartItem> items)
        {
            // 결제 창 기본 디자인 세팅
            this.Text = "주문 결제하기";
            this.Size = new Size(450, 550);
            this.StartPosition = FormStartPosition.CenterParent;
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;

            // 상단 제목
            Label lblTitle = new Label() { Text = "주문 내역 확인",
                Font = new Font("맑은 고딕", 16, FontStyle.Bold),
                Location = new Point(20, 20),
                Size = new Size(200, 30)
            };
            this.Controls.Add(lblTitle);

            // 주문 목록 리스트박스
            ListBox listBox = new ListBox() { Location = new Point(20, 60),
                Size = new Size(390, 240),
                Font = new Font("맑은 고딕", 11)
            };

            int totalQty = 0;
            int totalPrice = 0;

            foreach (var item in items)
            {
                listBox.Items.Add($"{item.Name}   {item.Quantity}개   -   {item.TotalPrice:N0}원");
                totalQty += item.Quantity;
                totalPrice += item.TotalPrice;
            }
            this.Controls.Add(listBox);

            // 총 수량 및 금액 요약 레이블 
            Label lblSummary = new Label()
            {
                Text = $"총 수량: {totalQty}개\n총 결제금액: {totalPrice:N0}원",
                Font = new Font("맑은 고딕", 13, FontStyle.Bold),
                ForeColor = Color.DarkRed,
                Location = new Point(20, 315),
                Size = new Size(390, 60),
                TextAlign = ContentAlignment.MiddleRight
            };
            this.Controls.Add(lblSummary);

            // 결제 방법 선택 제목
            Label lblMethod = new Label() { Text = "결제 수단 선택",
                Font = new Font("맑은 고딕", 12, FontStyle.Bold),
                Location = new Point(20, 395),
                Size = new Size(200, 25) };
            this.Controls.Add(lblMethod);

            // 신용카드 결제 버튼
            Button btnCard = new Button() { Text = "신용카드",
                Location = new Point(20, 435),
                Size = new Size(120, 50),
                Font = new Font("맑은 고딕", 11, FontStyle.Bold),
                BackColor = Color.LightSkyBlue, FlatStyle = FlatStyle.Flat
            };

            btnCard.Click += (s, e) =>
            {
                MessageBox.Show("카드를 투입구에 끝까지 넣어주세요...\n\n결제가 완료되었습니다!", "결제 성공", MessageBoxButtons.OK, MessageBoxIcon.Information);
                this.DialogResult = DialogResult.OK;
                this.Close();
            };

            // 간편결제 버튼
            Button btnEasyPay = new Button() { Text = "간편결제\n(QR/바코드)",
                Location = new Point(155, 435),
                Size = new Size(120, 50),
                Font = new Font("맑은 고딕", 10, FontStyle.Bold),
                BackColor = Color.LightGreen,
                FlatStyle = FlatStyle.Flat };

            btnEasyPay.Click += (s, e) =>
            {
                MessageBox.Show("바코드 또는 QR코드를 스캐너에 대어주세요...\n\n결제가 완료되었습니다!", "결제 성공", MessageBoxButtons.OK, MessageBoxIcon.Information);
                this.DialogResult = DialogResult.OK;
                this.Close();
            };

            // 취소 버튼
            Button btnCancel = new Button() { Text = "취소하기",
                Location = new Point(290, 435),
                Size = new Size(120, 50), Font = new Font("맑은 고딕", 11, FontStyle.Bold),
                BackColor = Color.LightCoral,
                FlatStyle = FlatStyle.Flat };

            btnCancel.Click += (s, e) =>
            {
                this.DialogResult = DialogResult.Cancel;
                this.Close();
            };

            this.Controls.Add(btnCard);
            this.Controls.Add(btnEasyPay);
            this.Controls.Add(btnCancel);
        }
    }
}