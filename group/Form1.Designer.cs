namespace Kiosk
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.Coffee = new System.Windows.Forms.Button();
            this.Food = new System.Windows.Forms.Button();
            this.NonCoffee = new System.Windows.Forms.Button();
            this.Home = new System.Windows.Forms.Button();
            this.Pay = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // Coffee
            // 
            this.Coffee.Location = new System.Drawing.Point(33, 43);
            this.Coffee.Name = "Coffee";
            this.Coffee.Size = new System.Drawing.Size(75, 23);
            this.Coffee.TabIndex = 0;
            this.Coffee.Text = "커피";
            this.Coffee.UseVisualStyleBackColor = true;
            this.Coffee.Click += new System.EventHandler(this.coffee_Click);
            // 
            // Food
            // 
            this.Food.Location = new System.Drawing.Point(33, 177);
            this.Food.Name = "Food";
            this.Food.Size = new System.Drawing.Size(75, 23);
            this.Food.TabIndex = 1;
            this.Food.Text = "음식";
            this.Food.UseVisualStyleBackColor = true;
            // 
            // NonCoffee
            // 
            this.NonCoffee.Location = new System.Drawing.Point(33, 110);
            this.NonCoffee.Name = "NonCoffee";
            this.NonCoffee.Size = new System.Drawing.Size(75, 23);
            this.NonCoffee.TabIndex = 2;
            this.NonCoffee.Text = "논커피";
            this.NonCoffee.UseVisualStyleBackColor = true;
            // 
            // Home
            // 
            this.Home.Location = new System.Drawing.Point(33, 392);
            this.Home.Name = "Home";
            this.Home.Size = new System.Drawing.Size(75, 23);
            this.Home.TabIndex = 3;
            this.Home.Text = "처을으로";
            this.Home.UseVisualStyleBackColor = true;
            // 
            // Pay
            // 
            this.Pay.Location = new System.Drawing.Point(33, 341);
            this.Pay.Name = "Pay";
            this.Pay.Size = new System.Drawing.Size(75, 23);
            this.Pay.TabIndex = 4;
            this.Pay.Text = "결제하기";
            this.Pay.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.Pay);
            this.Controls.Add(this.Home);
            this.Controls.Add(this.NonCoffee);
            this.Controls.Add(this.Food);
            this.Controls.Add(this.Coffee);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button Coffee;
        private System.Windows.Forms.Button Food;
        private System.Windows.Forms.Button NonCoffee;
        private System.Windows.Forms.Button Home;
        private System.Windows.Forms.Button Pay;
    }
}

