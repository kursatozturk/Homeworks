
const client = stitch.Stitch.initializeDefaultAppClient('not_ebay-cfagp');
stitch.Stitch.defaultAppClient.auth.loginWithCredential(new stitch.AnonymousCredential()).then(user => {
    console.log(`Logged in as anonymous user with id: ${user.id}`);
 }).catch(console.error);
 

Vue.component('user-viewer', {
    props:[
        'user'
    ],
    template: `
            <div class='col-sm-4'>
                {{ user.username }}
                Wallet: {{ user.wallet }}
                Store: {{ user.store }}
                Order History: {{ user.history }}
            </div>
    `
});

Vue.component('order-history', {
    props: [ 'purchase' ],
    methods: {
        rate_purchase(){
            this.$emit('rate_seller', this.purchase._id.toString(), this.rate);
        },
        seller_page(){
            this.$emit('seller', this.purchase.product.seller);
        }
    },
    data: function(){
        return {
            'rate': 0
        }
    },
    template: `
    <tr>
            <td> {{ purchase.product.product_name }} </td>
            <td> {{ purchase.product.price }} </td>
            <td> {{ purchase.product.quantity }} </td>
            <td> <a :href= "purchase.product.photo_link" target='_blank'> photo </a> </td>
            <td> <a href='#' @click='seller_page' > {{ purchase.product.seller }} </a> </td>
            <td> {{ purchase.product.rating }} </td>
            <select :disabled='purchase.rating' v-model="rate"> 
                <option 
                    v-for='i in 5'
                    :key='i'
                    :selected='purchase.rating == i'
                    >{{ i }}
                </option>
            </select>
            <td> <button class='btn btn-success' href="#" @click='rate_purchase' :disabled='purchase.rating'> Rate </button> </td>
    </tr>
`
});

Vue.component('product-viewer', {
    props: [
       'product'
    ],
    methods: {
        buy_item(){
            this.$emit('buy_item', this.product, this.quantity);
        },
        seller_page(){
            this.$emit('seller', this.product.seller);
        },
        quantity_arrange(){
            var range_q = [];
            if(this.product.quantity > 100){
                var incr = Math.floor(this.product.quantity/100);
                for(var i = 0; i < Math.min(incr, 100); i++){
                    range_q.push(i);
                }
                for(var i = Math.min(incr, 100); i < this.product.quantity; i += incr){
                    range_q.push(i);
                }
                return range_q;
            }
            else return this.product.quantity;
            
        }
    },
    data: function(){
        return {
            quantity: 0
        }
    },
    template: `
        <tr>
                <td> {{ product.product_name }} </td>
                <td> {{ product.price }} </td>
                <td> {{ product.quantity }} </td>
                <td> <a :href= "product.photo_link" target='_blank'> photo </a> </td>
                <td> <a href='#' @click='seller_page' > {{ product.seller }} </a> </td>
                <td> {{ product.rating }} </td>
                <td>
                    <select v-model='quantity'> 
                        <option 
                        v-for='n in quantity_arrange()'
                        :key='n'
                        :value='n'
                        > {{ n }}
                        </option>
                    </select> 
                    <button class='btn btn-success' href="#" @click="buy_item"> BUY </button> 
                </td>
        </tr>
    `
  });   


  Vue.component('userbar', {

    props:['user'],
    data: function(){
        return{
            deposit_money: 0,
            withdraw_money: 0
        }
    },
    methods:{
        home_page(){
            this.$emit('home_page', null);
        },
        store_page(){
            this.$emit('store', this.user.username);
        },
        order_history_page(){
            this.$emit('order_history', this.user.username);
        },
        putOnSale(){
            this.$emit('sale');
        },
        deposit(){
            this.$emit('deposit', this.deposit_money);
        },
        withdraw(){
            this.$emit('withdraw', this.withdraw_money);
        },
        logout(){
            this.$emit('logout');
        }


    },
    template: `
        <nav class="navbar navbar-expand-lg navbar navbar-dark bg-dark">
            <a class="navbar-brand" href="#">eBay</a>
            <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>

            <div class="collapse navbar-collapse flex-grow-1 text-left" id="navbarSupportedContent">
                <ul class="navbar-nav mr-auto flex-nowrap">
                    <li class="nav-item active">
                        <button class="btn btn-outline-success" type="button" @click='home_page' :disabled="user === null">Home <span class="sr-only">(current)</span></button>
                    </li>
                    <li class="nav-item active">
                        <button class="btn btn-outline-success" type="button" @click='store_page' :disabled="user === null">Store <span class="sr-only">(current)</span></button>
                    </li>
                    <li class="nav-item active">
                        <button class="btn btn-outline-success" type="button" @click='order_history_page' :disabled="user === null"> Order History <span class="sr-only">(current)</span></button>
                    <li class="nav-item active" >
                        <button class="btn btn-outline-success" type="button" @click='putOnSale' :disabled="user === null"> Put Product On Sale <span class="sr-only">(current)</span></button>
                    </li>
                    <li class="nav-item active" >
                        <input class='form-control' type='text' v-model='deposit_money'>
                        <button class="btn btn-outline-success" type="button" @click='deposit' :disabled="user === null"> Deposit Money <span class="sr-only">(current)</span></button>
                    </li>
                    <li class="nav-item active">
                        <input class='form-control' type='text' v-model='withdraw_money'>
                        <button class="btn btn-outline-success" type="button" @click='withdraw' :disabled="user === null"> Withdraw Money <span class="sr-only">(current)</span></button>
                    </li>
                    <li class='nav-item active'>
                        <button class="btn btn-outline-success" type="button" @click='logout' :disabled="user === null"> LogOut <span class="sr-only">(current)</span></button>

                    </li>
                </ul>
            </div>
        </nav>
    `
  });


  Vue.component('product-template', {
    data: function(){
        return {
            product: {}
        }
    },
    template: `
            <div class='field'>

                <label class="label" for='name'> Name: </label>
                <div class='control'>
                    <input class='input' name='name' v-model='product.name' >
                <div>
                <label class="label" for='price'> Price: </label>
                <div class='control'>
                <input class='input' name='price' v-model='product.price' >
                <div>
                <label class="label" for='quantity'> Quantity: </label>
                <div class='control'>
                <input class='input' name='quantity' v-model='product.quantity' >
                <div>
                <label class="label" for='photo-link'> Photo Link: </label>
                <div class='control'>
                <input class='input' name='photo-link' v-model='product.photo_link' >
                <div>
                <button class='btn btn-outline-danger' type='button' @click='create_product'> Put Product On Sale </button>
            </div>
    `,
    methods:{
        create_product(){
            this.$emit('product_creation', this.product);
        },
    }

  });

  Vue.component('user-info', {
    props: ['user'],
    methods: {
        
        order_history(){
            this.$emit('order_history',this.user.username);
        }
    },
    template: `
        <div class='col-sm-3'>
            <table class='table table-info'>
                <thead>
                <h1> {{ user.username }}</h1>
                </thead>
                <tbody>
                    <tr>
                        <th scope='row'>Wallet:</th>
                        <td>{{ user.wallet }}</td>
                    </tr>
                    <tr>
                        <th scope='row'>Rating:</th>
                        <td>{{ user.rating }}</td>
                    </tr>
                    <tr>
                        <th scope='row'>
                            Order History: 
                        </th>
                        <td>
                            <button class='btn btn-info' @click='order_history'> GO! </button>
                        </td>
                        
                    </tr>
                </tbody>
            </table>    
        </div>
    
    `

  })